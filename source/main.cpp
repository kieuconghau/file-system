#include "Program.h"
#include "Console.h"
#include <stdint.h>

int main() {
	srand((int)time(0));

	Program fileSystem;
	fileSystem.run();

	return 0;
}

