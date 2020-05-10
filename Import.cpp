#include "Import.h"

// Quick comparison of the content of our volume
// before and after importing a new file:
// * Before:
// [Data of old files] - [Entries of old files] - [Volume Info Area]
// * After:
// [Data of old files] - [Data of the new file(s)] - [Entries of old files]
// - [Entry (or entries) of the new file(s)] - [Volume Info Area]

// There are 4 steps to import a new file (or new files) to a volume:
// * Step 1: Store the info and data of the new file(s) into RAM.
// * Step 2: Temporarily store the content of the current
// Entry Table - which is containing entries of old files,
// and Volume Info Area of the volume into some buffer
// to move them later on.
// * Step 3: In the volume, write a bundle of
// "[Data of the new file(s)]
// - [Entries of old files] (which is the content of the Entry Table
// stored earlier)
// - [Entry (or entries) of the new file(s)] (consisting of
// the new file's (or new files') info)
// - [Volume Info Area]"
// after the [Data of old files] (see the comparison above if you don't
// understand).
// * Step 4: Update the info of the volume in Volume Info Area
// if necessary.

FileInfo GetFileInfo(_WIN32_FIND_DATAA ffd) {
	FileInfo file_info{ 0, 0 };

	return file_info;
}

FileEntry ConvertFileInfoToEntry(FileInfo file_info) {
	FileEntry file_entry{ false, 0, NULL, 0, NULL };

	return file_entry;
}

void Import(Volume volume, string new_file_path) {

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
	_WIN32_FIND_DATAA ffd;	// Data found using WinAPI finding function

	HANDLE hFile = FindFirstFileA(new_file_path.c_str(), &ffd);
	if (hFile == INVALID_HANDLE_VALUE) {
		cout << "Failed to find the file." << endl;
		return;
	}

	file_entry = ConvertFileInfoToEntry(GetFileInfo(ffd));
	file_entry_vector.push_back(file_entry);

	// If the current file is actually a folder
	if (ffd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
		queue<string> file_name_queue;
		string file_name;

		file_name_queue.push(new_file_path);

		while (file_name_queue.empty() == false) {
			file_name = file_name_queue.front() + "\\*";
			hFile = FindFirstFileA(file_name.c_str(), &ffd);
			do {

				file_entry = ConvertFileInfoToEntry(GetFileInfo(ffd));
				file_entry_vector.push_back(file_entry);

				// If the current file is a folder
				if (ffd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
					file_name_queue.push(ffd.cFileName);
				}

			} while (FindNextFileA(hFile, &ffd) != 0);

			file_name_queue.pop();
		}
	}

	FindClose(hFile);

	// Step 1.2: Get the data

	// Now we'll get the data of the files among the group
	// of files and folders in the directory tree we've just
	// finished traversing.
	// (Again, if we inputted in a normal file (not a folder)
	// then we'll just get the data of that file only)

	vector<FileData> file_data_vector;

	fstream file_stream;

	for (size_t i = 0; i < file_entry_vector.size(); i++) {
		if (file_entry_vector[i].is_entry_of_folder == true) {

			file_stream.open(file_entry_vector[i].file_name, ios::binary);
			if (file_stream.is_open() == false) continue;

			FileData file_data;
			file_data.file_size = file_entry_vector[i].file_size;
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

	// Set volume stream's pointer
	// to the beginning of Entry Table.
	volume.stream.seekg(volume.entry_table_offset);

	// Store the content of Entry Table into a char array.
	char* entry_table = new char[volume.entry_table_size];
	volume.stream.read(entry_table, volume.entry_table_size);

	// Now the stream's pointer is at the beginning of Volume Info Area.
	// Store the content of Volume Info Area into a char array.
	char* volume_info_area = new char[volume.volume_info_area_size];
	volume.stream.read(volume_info_area, volume.volume_info_area_size);

	// Step 3:

	// Set volume stream's pointer
	// to the beginning of Entry Table again.
	volume.stream.seekp(volume.entry_table_offset);

	// Write a bundle of
	// "[Data of the new file(s)]
	// - [Entries of old files] (which is the content of the Entry Table
	// stored earlier)
	// - [Entry (or entries) of the new file(s)] (consisting of
	// the new file's (or new files') info)
	// - [Volume Info Area]"
	for (size_t i = 0; i < file_data_vector.size(); i++) {
		volume.stream.write(file_data_vector[i].data,
			file_data_vector[i].file_size);
	}
	volume.stream.write(entry_table, volume.entry_table_size);
	for (size_t i = 0; i < file_data_vector.size(); i++) {
		volume.stream.write(file_entry_vector[i].entry,
			file_entry_vector[i].entry_size);
	}
	volume.stream.write(volume_info_area, volume.volume_info_area_size);

	// Step 4:

	// ...
	// (unsure about the content of Volume Info Area
	// so can't implement yet)

	// Extra step:
	// Deallocate all the memory we've borrowed from heap memory.

	for (size_t i = 0; i < file_data_vector.size(); i++) {
		delete[] file_data_vector[i].data;
	}

	for (size_t i = 0; i < file_entry_vector.size(); i++) {
		delete[] file_entry_vector[i].entry;
	}

	delete[] entry_table;
	delete[] volume_info_area;

}
