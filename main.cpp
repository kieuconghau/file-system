#include "Volume.h"
#include "GUI.h"

void createVolume_test(string const& pathVolumeFile, vector<Entry> const& entryList);

int main() {
	string const pathVolumeFile = "test";

	// Create volume
	vector<Entry> entryList = {
		Entry(120, "Folder1/", ""),
		Entry(50, "Folder1/Folder2/", ""),
		Entry(50, "Folder1/Folder2/Folder3/", ""),
		Entry(50, "Folder1/Folder2/Folder3/fileA.txt", ""),
		Entry(70, "Folder1/fileB.bin", ""),
		Entry(25, "fileC.abc", "")
	};

	createVolume_test(pathVolumeFile, entryList);

	// Read volume
	Volume vol;
	if (vol.isVolumeFile(pathVolumeFile)) {
		vol.open(pathVolumeFile);
	}

	return 0;
}

void createVolume_test(string const& pathVolumeFile, vector<Entry> const& entryList) {
	Volume vol;
	vol.create("test");

	for (Entry entry : entryList) {
		vol.addEntry_test(entry);
	}
}