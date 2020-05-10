#pragma once

#include <iostream>
#include <fstream>

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

void Import(Volume volume, string new_file_path);
