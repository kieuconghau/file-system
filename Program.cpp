#include "Program.h"

Program::Program()
{
	GUI::setWindows(W, H);
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
	this->Vol = new Volume();

	string volumeFilePath;
	cout << "Input path: ";
	getline(cin, volumeFilePath);

	if (this->Vol->isVolumeFile(volumeFilePath)) {
		this->Vol->open(volumeFilePath);
	}
	else {
		this->closeVolume();
	}
}

void Program::createVolume()
{
	this->Vol = new Volume();

	string volumeFilePath;
	cout << "Input path: ";
	getline(cin, volumeFilePath);

	this->Vol->create(volumeFilePath);

	this->closeVolume();
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
	gotoXY(0, 5); cout << "//       Nguyen Hoang Nhan - 18127xxx";
	gotoXY(0, 6); cout << "//       Kieu Cong Hau     - 18127xxx";
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
	gotoXY(48, 25); cout << " 1.>  Create a new volume   ";	setColor(15, 0);
	setColor(14, 0);
	if (GUI::line == 1) setColor(15, 1);
	gotoXY(48, 26); cout << " 2.> Open an existed volume "; setColor(15, 0);
	setColor(14, 0);
	if (GUI::line == 2) setColor(15, 1);
	gotoXY(48, 27); cout << " 3.>      Instruction       "; setColor(15, 0);
	setColor(14, 0);
	if (GUI::line == 3) setColor(15, 1);
	gotoXY(48, 28); cout << " 4.>         Exit           "; setColor(15, 0);
	setColor(15, 0);
}

void Program::instructionScreen() {


	gotoXY(0, 0);  cout << ".NHT INSTRUCTION";

	gotoXY(0, 2);  cout << "* Use ARROW 'UP' and ARROW 'DOWN' to move upward and downward between lines. ";

	gotoXY(0, 4);  cout << "* When you open an existed volume successfully these are functional key you should know:";

	gotoXY(0, 6);  cout << "\t\t[ENTER]:        To enter a folder.";

	gotoXY(0, 8);  cout << "\t\t[P]:            To set password for a folder or file.";
	gotoXY(0, 9);  cout << "\t\t                If a folder/file has password, it requires password to access(folder) or deletion.";
	gotoXY(0, 10); cout << "\t\t                If you delete a folder which has a locked file/folder inside,";


	gotoXY(8, 11); cout << "\t\t[DEL] or [D]:   To delete a file or a folder";


	system("pause");
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
					this->instructionScreen();
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