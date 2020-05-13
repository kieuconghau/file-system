#include "Program.h"

Program::Program()
{
	//GUI::setWindows(W, H);
	this->Vol = nullptr;
}

Program::~Program()
{
	delete this->Vol;
}

void Program::run()
{
	this->homeNavigate();
}

void Program::openVolume()
{
	setColor(COLOR::LIGHT_CYAN, COLOR::BLACK);
	cout << "::::: OPEN A VOLUME :::::";
	cout << "\n\n";

	// Show list of existing volume path in cache
	setColor(COLOR::WHITE, COLOR::BLACK);
	vector<string> volumePathList = this->Cache.getVolumePathList();
	for (size_t i = 1; i <= volumePathList.size(); ++i) {
		cout << " <" << i << "> ";
		gotoXY(7, whereY());
		cout << volumePathList[i - 1] << "\n";
	}
	cout << "\n";

	// Input
	setColor(COLOR::LIGHT_CYAN, COLOR::BLACK);
	if (this->Cache.isEmpty()) {
		cout << "Program: Input a path." << "\n";
	}
	else {
		cout << "Program: Input a path (or <i>, with i is the index of the above list)." << "\n";
	}
	cout << "\n" << "Your input: ";

	setColor(COLOR::WHITE, COLOR::BLACK);
	string str;
	getline(cin, str);

	// If the input is <i>, change the content of str to the correspoding volume path
	bool openVolumeInCache = false;

	if (str.length() != 0 && str.front() == '<' && str.back() == '>') {
		size_t indexVolume = 0;

		for (size_t i = 1; i < str.length() - 1; ++i) {
			if (str[i] >= '0' && str[i] <= '9') {
				indexVolume *= 10;
				indexVolume += str[i] - '0';
			}
			else {
				indexVolume = 0;
				break;
			}
		}

		if (indexVolume >= 1 && indexVolume <= volumePathList.size()) {
			openVolumeInCache = true;
			str = volumePathList[indexVolume - 1];
		}
	}

	// Open volume
	this->initializeVolume(str);

	if (this->Vol->isVolumeFile()) {
		this->Cache.add(this->Vol->getPath());

		this->Vol->open();
	}
	else {
		setColor(14, 0);
		cout << endl;
		cout << "Program: Cannot open this volume." << endl << endl;

		setColor(15, 0);
		system("pause");
	}

	this->closeVolume();

	if (openVolumeInCache) {
		this->Cache.update();
	}
}

void Program::createVolume()
{
	setColor(COLOR::LIGHT_CYAN, COLOR::BLACK);
	cout << "::::: CREATE A VOLUME :::::";
	cout << "\n\n";

	cout << "Program: Input a path." << "\n\n";
	cout << "Your input: ";

	setColor(COLOR::WHITE, COLOR::BLACK);
	string volumeFilePath;
	getline(cin, volumeFilePath);

	this->initializeVolume(volumeFilePath);

	if (this->Vol->create()) {
		this->Cache.add(this->Vol->getPath());

		setColor(10, 0);
		cout << endl;
		cout << "Program: The new volume is created successfully!" << endl << endl;
		
		setColor(15, 0);
		system("pause");
	}
	else {
		setColor(14, 0);
		cout << endl;
		cout << "Program: Cannot create a new volume." << endl << endl;
		cout << "         This path does not exist OR it is taken." << endl << endl;
		cout << "         Please choose another path." << endl << endl;
		
		setColor(15, 0);
		system("pause");
	}

	this->closeVolume();
}

void Program::initializeVolume(string const& volumeFilePath)
{
	this->Vol = new Volume(volumeFilePath);
}

void Program::closeVolume()
{
	delete this->Vol;
	this->Vol = nullptr;
}

void Program::homeScreen() {
	setColor(14, 0);
	gotoXY(0, 0); cout << "// /////////////////////////////////////////////";
	gotoXY(0, 1); cout << "// University of Science - Ho Chi Minh City";
	gotoXY(0, 2); cout << "// fit - Operating System";
	gotoXY(0, 3); cout << "// Project 21.";
	gotoXY(0, 4); cout << "// From: ";
	gotoXY(0, 5); cout << "//       Nguyen Hoang Nhan - 18127017";
	gotoXY(0, 6); cout << "//       Kieu Cong Hau     - 18127259";
	gotoXY(0, 7); cout << "//       Tran Thanh Tam    - 18127268";
	gotoXY(0, 8); cout << "//";

	setColor(12, 0);
	gotoXY(43, 11); cout << ".......##....##..##.....##..########";
	gotoXY(43, 12); cout << ".......###...##..##.....##.....##...";
	gotoXY(43, 13); cout << ".......####..##..##.....##.....##...";
	gotoXY(43, 14); cout << ".......##.##.##..#########.....##...";
	gotoXY(43, 15); cout << ".......##..####..##.....##.....##...";
	gotoXY(43, 16); cout << "...##..##..####..##.....##.....##...";
	gotoXY(43, 17); cout << "..###..##....##..##.....##.....##...";
	
	setColor(14, 0);
	gotoXY(73, 19); cout << "Version: Beta";
	gotoXY(73, 20); cout << "Release: 16.05.2020";

	setColor(12, 0);
	gotoXY(36, 23); cout << " .================================================." << endl;
	gotoXY(36, 24); cout << "||                                                ||" << endl;
	gotoXY(36, 25); cout << "||                                                ||" << endl;
	gotoXY(36, 26); cout << "||                                                ||" << endl;
	gotoXY(36, 27); cout << "||                                                ||" << endl;
	gotoXY(36, 28); cout << "||                                                ||" << endl;
	gotoXY(36, 29); cout << "||                                                ||" << endl;
	gotoXY(36, 30); cout << " *================================================*" << endl;
	
	setColor(14, 0);
	if (GUI::line == 0) setColor(15, 1);
	gotoXY(48, 25); cout << " 1.>   Create a new volume   ";	setColor(15, 0);
	setColor(14, 0);
	if (GUI::line == 1) setColor(15, 1);
	gotoXY(48, 26); cout << " 2.> Open an existing volume "; setColor(15, 0);
	setColor(14, 0);
	if (GUI::line == 2) setColor(15, 1);
	gotoXY(48, 27); cout << " 3.>      Instruction       "; setColor(15, 0);
	setColor(14, 0);
	if (GUI::line == 3) setColor(15, 1);
	gotoXY(48, 28); cout << " 4.>         Exit           "; setColor(15, 0);
	setColor(15, 0);
}

void Program::homeNavigate() {
	clrscr();
	GUI::clearBackground();

	bool back = false;

	this->homeScreen();

	GUI::line = 0;

	while (true) {

		if (_kbhit()) {
			FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

			// ============= ENTER =============
			if (GetKeyState(0x0D) & 0x8000) {
				while ((GetAsyncKeyState(VK_RETURN) & 0x8000)) {};

				clrscr();
				GUI::clearBackground();

				FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

				switch (GUI::line) {
				case 0:
					this->createVolume();
					break;
				case 1:
					GUI::reset();
					this->openVolume();
					break;
				case 2:
					GUI::instructionScreen();
					break;
				case 3:
					back = true;
					break;
				}
				GUI::reset();

				GUI::esc = false;
			}

			// ============= UP =============
			if (GetKeyState(VK_UP) & 0x8000) {
				if (GUI::line == 0) {
					GUI::line = 3;
				}
				else GUI::line--;
				GUI::line %= 4;
			}

			// ============= DOWN =============
			if (GetKeyState(VK_DOWN) & 0x8000) {
				GUI::line++;
				GUI::line %= 4;
			}

			// ============= EXIT =============
			if (GetKeyState(0x1B) & 0x8000) {
				while ((GetAsyncKeyState(0x1B) & 0x8000)) {};

				back = true;
			}

			// Refresh 
			if (!GUI::esc) {
				clrscr();
				GUI::clearBackground();
				homeScreen();
			}

			if (back) {
				break;
			}
		}

	}
}