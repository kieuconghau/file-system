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
	uint16_t modified_time;
	uint16_t modified_date;
	uint32_t file_size;
	uint16_t file_name_length;
	uint16_t file_password_length;
	uint32_t file_offset;
	char* file_name = NULL;
	char* file_password = NULL;
};

// Placeholder struct
struct FileEntry {
	FileInfo file_info;
	string file_path;
	bool is_entry_of_folder;
	int entry_size;
	char* entry = NULL;
};

// Placeholder struct
struct FileData {
	int file_size;
	char* data = NULL;
};

FileEntry GetFileInfoAndConvertToEntry(_WIN32_FIND_DATAA ffd,
	string file_path, string file_name_in_volume,
	uint32_t &insert_pos);
void Import(Volume &volume, string new_file_path);
