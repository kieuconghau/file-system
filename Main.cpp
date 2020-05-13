#include "Import.h"
#include <string>

int main() {

	string volume_path;
	cout << "Volume path: ";
	getline(cin, volume_path);

	Volume volume;
	volume.stream.open(volume_path, ios::in | ios::out | ios::binary);
	if (volume.stream.is_open() == false) {
		cout << "Failed to open volume." << endl;
		return 1;
	}

	char signature[4];
	
	volume.stream.seekg(0, ios::beg);
	do {
		cout << "Offset: " << volume.stream.tellg() << endl;
		volume.stream.read(signature, sizeof(signature));
		volume.stream.seekg(- (int)sizeof(signature) + 1, ios::cur);
	} while ((signature[0] != 'T' || signature[1] != 'H'
		|| signature[2] != 'N' || signature[3] != ' ')
		&& volume.stream.eof() == false);

	if ((signature[0] != 'T' || signature[1] != 'H'
		|| signature[2] != 'N' || signature[3] != ' ')) {
		cout << "Not a volume" << endl;
		return 2;
	}

	volume.stream.seekg(-1, ios::cur);
	volume.volume_info_area_offset = volume.stream.tellg();

	volume.stream.seekg(sizeof(signature), ios::cur);
	volume.stream.read((char*)&volume.entry_table_size,
		sizeof(volume.entry_table_size));

	volume.stream.read((char*)&volume.entry_table_offset,
		sizeof(volume.entry_table_offset));

	if (volume.entry_table_offset == UINT32_MAX) {
		volume.is_empty = true;
	}
	else {
		volume.is_empty = false;
	}

	string new_file_path;
	cout << "Importing file path: ";
	getline(cin, new_file_path);

	Import(volume, new_file_path);

	volume.stream.close();

	return 0;
}
