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

	int empty;
	cout << "Is the volume empty?" << endl;
	cin >> empty;
	if (empty == 0) {
		volume.is_empty = false;
	}
	else {
		volume.is_empty = true;
	}

	cout << "Volume Info Area offset: ";
	cin >> volume.volume_info_area_offset;

	cout << "Volume Info Area size: ";
	cin >> volume.volume_info_area_size;

	cout << "Entry Table offset: ";
	cin >> volume.entry_table_offset;

	cout << "Entry Table size: ";
	cin >> volume.entry_table_offset;

	string new_file_path;
	cout << "Importing file path: ";
	cin.ignore();
	getline(cin, new_file_path);

	Import(volume, new_file_path);

	volume.stream.close();

	return 0;
}
