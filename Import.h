#pragma once

#include <iostream>
#include <fstream>

using namespace std;

// Placeholder struct
struct Volume {
	fstream stream;
	int volume_info_area_offset;
	int volume_info_area_size;
	int file_directory_area_offset;
	int file_directory_area_size;
};

// Placeholder struct
struct FileInfo {
	int entry_size;
	int file_size;
};

void Import(Volume volume, string new_file_path);
