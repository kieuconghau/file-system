#include "Volume.h"

int main() {
	Volume vol;

	if (vol.isVolumeFile("test"))
		vol.open("test");
	else
		cout << "Can not open this volume." << endl;
	
	return 0;
}