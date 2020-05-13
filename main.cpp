#include "Program.h"
#include "GUI.h"
#include <stdint.h>

int main() {
	srand((int)time(0));

	Program fileSystem;
	fileSystem.run();

	return 0;
}

