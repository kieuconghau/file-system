#include "Import.h"

// Quick comparison of the content of our volume
// before and after importing a new file:
// * Before:
// [Data of old files] - [Entries of old files] - [Volume Info Area]
// * After:
// [Data of old files] - [Data of the new file(s)] - [Entries of old files]
// - [Entry (or entries) of the new file(s)] - [Volume Info Area]

// There are 3 steps to import a new file (or new files) to a volume:
// * Step 1: Store the info and data of the new file(s) into RAM.
// * Step 2: Temporarily store the content of the current
// Entry Table - which is containing entries of old files,
// and Volume Info Area of the volume into some buffer
// to move them later on.
// * Step 3: In the volume, overwrite the content
// from the end of [Data of old files] to the end of volume
// by some new content. Those new content is a bundle of:
// "[Data of the new file(s)]
// - [Entries of old files] (which is the content of the Entry Table
// stored earlier)
// - [Entry (or entries) of the new file(s)] (consisting of
// the new file's (or new files') info)
// - [Volume Info Area]"
// (see the comparison above if you don't understand).
// During the process of writing the bundle, we'll also update the info
// of the volume in Volume Info Area.

FileEntry GetFileInfoAndConvertToEntry(_WIN32_FIND_DATAA ffd,
	string file_path, string file_name_in_volume,
	uint32_t &insert_pos) {
	FileInfo file_info;

	// File last modification date and time.
	FileTimeToDosDateTime(&ffd.ftLastWriteTime, &file_info.modified_date,
		&file_info.modified_time);

	// File size.
	// If file size is not greater than (2^32 - 1)
	// (which means nFileSizeHigh == 0), get
	// file size in nFileSizeLow and store it.
	if (ffd.nFileSizeHigh == 0) {
		file_info.file_size = ffd.nFileSizeLow;
	}
	// Else, just store file size as (2^32 - 1)
	else {
		file_info.file_size = UINT32_MAX;
	}

	// File name length.
	file_info.file_name_length = file_name_in_volume.length();

	// File password length.
	file_info.file_password_length = 0;

	// File offset.
	file_info.file_offset = insert_pos;
	insert_pos += file_info.file_size;

	// File name.
	file_info.file_name = new char[file_info.file_name_length + 1];
	strcpy(file_info.file_name, file_name_in_volume.c_str());

	FileEntry file_entry;

	file_entry.file_info = file_info;

	file_entry.file_path = file_path;

	if (ffd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
		file_entry.is_entry_of_folder = true;
	}
	else {
		file_entry.is_entry_of_folder = false;
	}

	file_entry.entry_size = sizeof(file_info.modified_time)
		+ sizeof(file_info.modified_date)
		+ sizeof(file_info.file_size)
		+ sizeof(file_info.file_name_length)
		+ sizeof(file_info.file_password_length)
		+ sizeof(file_info.file_offset)
		+ file_info.file_name_length;

	file_entry.entry = new char[file_entry.entry_size];

	char* p = file_entry.entry;

	memmove(p, (char*)&file_info.modified_time,
		sizeof(file_info.modified_time));

	p += sizeof(file_info.modified_time);
	memmove(p, (char*)&file_info.modified_date,
		sizeof(file_info.modified_date));

	p += sizeof(file_info.modified_date);
	memmove(p, (char*)&file_info.file_size,
		sizeof(file_info.file_size));

	p += sizeof(file_info.file_size);
	memmove(p, (char*)&file_info.file_name_length,
		sizeof(file_info.file_name_length));

	p += sizeof(file_info.file_name_length);
	memmove(p, (char*)&file_info.file_password_length,
		sizeof(file_info.file_password_length));

	p += sizeof(file_info.file_password_length);
	memmove(p, (char*)&file_info.file_offset,
		sizeof(file_info.file_offset));

	p += sizeof(file_info.file_offset);
	strncpy(p, file_info.file_name, file_info.file_name_length);

	return file_entry;
}

void Import(Volume &volume, string new_file_path) {

	// Step 1: We'll get and store the info and data of the new file(s)
	// into our program.

	// Step 1.1: Get the info.

	// If the file we inputted is just a file (not a folder),
	// we'll get only the info of that file. But if the file
	// we inputted in is in fact a folder, then:

	// We'll use "level order tree traversal" to travel to each
	// and every file or folder in the "directory tree"
	// rooted at the file we inputted in.
	// Each time we arrive at a file or folder in the tree,
	// we'll get the info of that file or folder.

	vector<FileEntry> file_entry_vector;

	FileEntry file_entry;
	_WIN32_FIND_DATAA ffd;	// Data found using WinAPI finding function.

	// We'll use a variable to keep track of the position
	// where data of a new file is inserted. At first its value
	// will be the original offset of the Entry Table (which is also
	// the end position of the Data Area). Every time a file
	// is found and an entry is created, the value of this variable
	// will be used to fill in the "file offset" field of the entry
	// and then be updated.

	uint32_t insert_pos;
	if (volume.is_empty == false) {
		insert_pos = volume.entry_table_offset;
	}
	else {
		insert_pos = 0;
	}

	HANDLE hFile = FindFirstFileA(new_file_path.c_str(), &ffd);
	if (hFile == INVALID_HANDLE_VALUE) {
		cout << "Failed to find the file." << endl;
		return;
	}

	// If the current file is actually a folder.
	if (ffd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {

		queue<string> folder_path_queue;

		const string root_folder_path = new_file_path;
		const string root_folder_name = ffd.cFileName;
		string parent_folder_path;
		string file_name_in_volume;

		file_name_in_volume = root_folder_name;
		file_name_in_volume += "\\";

		file_entry = GetFileInfoAndConvertToEntry(ffd,
			root_folder_path, file_name_in_volume, insert_pos);
		file_entry_vector.push_back(file_entry);

		folder_path_queue.push(root_folder_path);

		while (folder_path_queue.empty() == false) {

			parent_folder_path = folder_path_queue.front() + "\\*";
			folder_path_queue.pop();

			hFile = FindFirstFileA(parent_folder_path.c_str(), &ffd);

			// If FindFirstFileA func can't find any file
			// (in other words, this "directory tree" has
			// just an empty folder at the root).
			if (hFile == INVALID_HANDLE_VALUE) continue;

			do {

				string file_path = parent_folder_path.substr(
					0, parent_folder_path.length() - 1) + ffd.cFileName;

				file_name_in_volume =
					parent_folder_path.substr(
						root_folder_path.length() - root_folder_name.length(),
						parent_folder_path.length()
						- root_folder_path.length()
						+ root_folder_name.length()
						- 1)
					+ ffd.cFileName;

				file_entry = GetFileInfoAndConvertToEntry(ffd,
					file_path, file_name_in_volume, insert_pos);
				file_entry_vector.push_back(file_entry);

				// If the current file is a folder.
				if (ffd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
					// Get the parent folder's path, remove the asterisk
					// at the tail, append the current folder's name,
					// and push the whole path into the queue.
					folder_path_queue.push(
						parent_folder_path.substr(0,
							parent_folder_path.length() - 1)
						+ ffd.cFileName);
				}

			} while (FindNextFileA(hFile, &ffd) != 0);

		}
	}
	else {
		file_entry = GetFileInfoAndConvertToEntry(ffd, new_file_path,
			ffd.cFileName, insert_pos);
		file_entry_vector.push_back(file_entry);
	}

	FindClose(hFile);

	// Step 1.2: Get the data

	// Now we'll get the data of the files among the group
	// of files and folders in the directory tree we've just
	// finished traversing.
	// (Again, if we inputted in a normal file (not a folder)
	// then we'll just get the data of that file only.)

	vector<FileData> file_data_vector;

	fstream file_stream;

	for (size_t i = 0; i < file_entry_vector.size(); i++) {

		// We only get the data of the files which are not folders.
		if (file_entry_vector[i].is_entry_of_folder == false) {

			file_stream.open(file_entry_vector[i].file_path,
				ios::binary);
			if (file_stream.is_open() == false) continue;

			FileData file_data;
			file_data.file_size =
				file_entry_vector[i].file_info.file_size;
			file_data.data = new char[file_data.file_size];
			file_stream.read(file_data.data, file_data.file_size);
			file_data_vector.push_back(file_data);

			file_stream.close();

		}
	}

	// All the info and data of the new file(s) have been prepared
	// and are waiting to be written to the volume.

	// Now we'll move on to Step 2:
	// Temporarily store the content of Entry Table
	// and Volume Info Area into some buffer, because later on
	// they'll be moved to some different places to make space
	// for the info and data of the new file(s).

	char* entry_table = NULL;

	// If the volume is empty (which means there's no Entry Table),
	// we don't have to do the step of moving Entry Table.

	if (volume.is_empty == false) {

		// Set volume stream's pointer
		// to the beginning of Entry Table.
		volume.stream.seekg(volume.entry_table_offset);

		// Store the content of Entry Table into a char array.
		entry_table = new char[volume.entry_table_size];
		volume.stream.read(entry_table, volume.entry_table_size);

	}
	else {
		volume.stream.seekg(volume.volume_info_area_offset);
	}

	// Now the stream's pointer is at the beginning of Volume Info Area.
	// Store the content of Volume Info Area into a char array.
	char* volume_info_area = new char[volume.volume_info_area_size];
	volume.stream.read(volume_info_area, volume.volume_info_area_size);

	// Step 3:

	if (volume.is_empty == false) {	// If the volume is not empty.

		// Set volume stream's pointer
		// to the beginning of Entry Table.
		volume.stream.seekp(volume.entry_table_offset);
	}
	else {	// If the volume is empty

		// Set volume stream's pointer
		// to the beginning of Volume Info Area
		// (which is also the beginning of the volume).
		volume.stream.seekp(volume.volume_info_area_offset);
	}

	// Write a bundle of
	// "[Data of the new file(s)]
	// - [Entries of old files] (which is the content of the Entry Table
	// stored earlier)
	// - [Entry (or entries) of the new file(s)] (consisting of
	// the new file's (or new files') info)
	// - [Volume Info Area]"
	// and update the volume's info in the Volume Info Area.

	// Write data of the new file(s).
	for (size_t i = 0; i < file_data_vector.size(); i++) {
		volume.stream.write(file_data_vector[i].data,
			file_data_vector[i].file_size);
	}

	// Update the offset of Entry Table.
	volume.entry_table_offset = volume.stream.tellp();

	// Write entries of the old files (which is the old
	// Entry Table).
	volume.stream.write(entry_table, volume.entry_table_size);

	// Write entry (or entries) of the new file(s).
	for (size_t i = 0; i < file_data_vector.size(); i++) {
		volume.stream.write(file_entry_vector[i].entry,
			file_entry_vector[i].entry_size);
	}

	// Update the size of Entry Table.
	volume.entry_table_size = (int)volume.stream.tellp()
		- volume.entry_table_offset;

	// Update the offset of Volume Info Area.
	volume.volume_info_area_offset = volume.stream.tellp();

	// Write the Volume Info Area
	volume.stream.write(volume_info_area, volume.volume_info_area_size);

	// Update the "empty" state of the volume
	if (volume.is_empty == true) {
		volume.is_empty = false;
	}

	// Extra step:
	// Deallocate all the memory we've borrowed from heap memory.

	for (size_t i = 0; i < file_data_vector.size(); i++) {
		if (file_data_vector[i].data != NULL) {
			delete[] file_data_vector[i].data;
		}
	}

	for (size_t i = 0; i < file_entry_vector.size(); i++) {
		if (file_entry_vector[i].file_info.file_name != NULL) {
			delete[] file_entry_vector[i].file_info.file_name;
		}
		if (file_entry_vector[i].entry != NULL) {
			delete[] file_entry_vector[i].entry;
		}	
	}

	if (entry_table != NULL) {
		delete[] entry_table;
	}

	delete[] volume_info_area;

}
