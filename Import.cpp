#include "Import.h"

// Quick comparison of the content of our volume
// before and after importing a new file:
// * Before:
// [Data of old files] - [Entries of old files] - [Volume Info Area]
// * After:
// [Data of old files] - [Data of the new file] - [Entries of old files]
// - [Entry of the new file] - [Volume Info Area]

// There are 4 steps to import a new file to a volume:
// * Step 1: Store the info and data of the new file into RAM.
// * Step 2: Temporarily store the content of the current
// Entry Table - which is containing entries of old files,
// and Volume Info Area of the volume into some buffer
// to move them later on.
// * Step 3: In the volume, write a bundle of
// "[Data of the new file]
// - [Entries of old files] (which is the content of the Entry Table
// stored earlier)
// - [Entry of the new file] (consisting of the new file's info)
// - [Volume Info Area]"
// after the [Data of old files] (see the comparison above if you don't
// understand).
// * Step 4: Update the info of the volume in Volume Info Area
// if necessary.

void Import(Volume volume, string new_file_path) {
	
	// Open new file.
	ifstream new_file_stream(new_file_path, ios::binary);
	if (new_file_stream.is_open() == false) return;

	// Step 1: We'll get and store the info and data of the new file
	// into our program.

	// Get the new file's info
	// and store them to a FileInfo object.
	FileInfo new_file_info;
	new_file_info = { 0, 0 };	// Just a placeholder line
								// to make the compiler works
	// ...
	// (don't know how to get info of a file in Windows
	// so can't implement yet)

	// Convert the new file's info in the FileInfo object
	// into an entry in the form of a char array.
	// This entry later on will be written to the
	// Entry Table in the volume.
	char* new_file_entry = new char[new_file_info.entry_size];
	// ...
	// (unsure about the list of a file's info
	// so can't implement yet)

	// Store the data of the new file into a char array.
	char* new_file_data = new char[new_file_info.file_size];
	new_file_stream.read(new_file_data, new_file_info.file_size);

	// All the info and data of the new file has been prepared
	// and is waiting to be written to the volume.

	// Now we'll move on to Step 2:
	// Temporarily store the content of Entry Table
	// and Volume Info Area into some buffer, because later on
	// they'll be moved to some different places to make space
	// for the info and data of the new file.

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
	// "[Data of the new file]
	// - [Entries of old files] (which is the content of the Entry Table
	// stored earlier)
	// - [Entry of the new file] (consisting of the new file's info)
	// - [Volume Info Area]"
	volume.stream.write(new_file_data, new_file_info.file_size);
	volume.stream.write(entry_table, volume.entry_table_size);
	volume.stream.write(new_file_entry, new_file_info.entry_size);
	volume.stream.write(volume_info_area, volume.volume_info_area_size);

	// Step 4:

	// ...
	// (unsure about the content of Volume Info Area
	// so can't implement yet)

	// Finally, close the new file.
	new_file_stream.close();
}
