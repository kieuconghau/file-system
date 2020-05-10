#pragma once

#include <iostream>
#include <fstream>
#include <Windows.h>
#include <queue>

using namespace std;

// Placeholder struct
struct Volume {
	fstream stream;
	int volume_info_area_offset;
	int volume_info_area_size;
	int entry_table_offset;
	int entry_table_size;
};

// Placeholder struct
struct FileInfo {
	int entry_size;
	int file_size;
};

// Placeholder struct
struct FileEntry {
	bool is_entry_of_folder;
	int entry_size;
	char* file_name;
	int file_size;
	char* entry;
};

// Placeholder struct
struct FileData {
	int file_size;
	char* data;
};

FileInfo GetFileInfo(_WIN32_FIND_DATAA ffd);
FileEntry ConvertFileInfoToEntry(FileInfo file_info);
void Import(Volume volume, string new_file_path);
