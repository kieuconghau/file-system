#include "Volume.h"

int main() {
	Volume vol;

	if (vol.isVolumeFile("test"))
		vol.open("test");

	return 0;
}