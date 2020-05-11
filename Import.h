#pragma once

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <queue>

using namespace std;

// Placeholder struct
struct Volume {
	fstream stream;
	bool is_empty;	// An empty volume has no data in Data Area
					// and no Entry Table, and has only
					// the Volume Info Area.
	int volume_info_area_offset;
	int volume_info_area_size;
	int entry_table_offset;
	int entry_table_size;
};

// Placeholder struct
struct FileInfo {
	char* file_name = NULL;
	int file_size;
};

// Placeholder struct
struct FileEntry {
	FileInfo file_info;
	bool is_entry_of_folder;
	int entry_size;
	char* entry = NULL;
};

// Placeholder struct
struct FileData {
	int file_size;
	char* data = NULL;
};

FileEntry GetFileInfoAndConvertToEntry(_WIN32_FIND_DATAA ffd);
void Import(Volume volume, string new_file_path);
