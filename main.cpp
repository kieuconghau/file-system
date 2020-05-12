#include "Volume.h"
#include "GUI.h"

int main() {
	Volume vol;

	string const pathVolumeFile = "test";
  
	if (vol.isVolumeFile(pathVolumeFile)) {
		vol.open(pathVolumeFile);
	}
	else {
		cout << "Can not open this volume." << endl;
	}
  
	return 0;
}

