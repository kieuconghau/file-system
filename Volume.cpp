#include "Volume.h"

Volume::Volume() : VolumeInfo(), EntryTable()
{
	this->Path = "";
}

Volume::~Volume() {}

void Volume::create(string const& volumeFilePath)
{
	this->Path = volumeFilePath;

	fstream file(this->Path, ios_base::out);
	if (file.is_open()) {
		file.clear();
		this->VolumeInfo.write(file);
	}
	file.close();
}

void Volume::open(string const& volumeFilePath)
{
	this->Path = volumeFilePath;
	
	fstream file(this->Path);
	if (file.is_open()) {
		file.clear();
		this->seekToHeadOfVolumeInfo(file);
		this->VolumeInfo.read(file);

		this->seekToHeadOfEntryTable(file);
		this->EntryTable.read(file, this->VolumeInfo);
	}
	file.close();

	this->performFunctions();
}

bool Volume::isVolumeFile(string const& volumeFilePath)
{
	bool isVF = false;
	this->Path = volumeFilePath;

	fstream file(this->Path, ios_base::in);
	if (file.is_open()) {
		file.clear();
		this->seekToHeadOfVolumeInfo(file);
		this->VolumeInfo.read(file);
		isVF = this->VolumeInfo.checkSignature(file);
	}
	file.close();

	return isVF;
}
  
void Volume::performFunctions()
{
	this->navigate(this->EntryTable.Root);
}

void Volume::navigate(Entry* f) {
	// Check if f is nullptr
	if (!f) return;

	char x = 0;
	bool back = false;

	// Ridiculous error fix
	GUI::clearBackground();

	// Reset line
	GUI::reset();
	this->updateMenu(f);

	while (true) {

		if (_kbhit()) {
			FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

			// ============= ENTER =============
			if (GetKeyState(0x0D) & 0x8000) {
				while ((GetAsyncKeyState(VK_RETURN) & 0x8000)) {};

				/*
				if (line != 0) {
					if (f->getEntryInList(line - 1)->isFolder())
						Navigation(f->getEntryInList(line - 1));
				}
				else {
					back = true;
					reset();
				}
				*/
				this->enterFolder(f, back);
			}

			// ============= UP =============
			if (GetKeyState(VK_UP) & 0x8000) {
				if (GUI::line == 0) {
					GUI::line = f->getListSize();
				}
				else GUI::line--;
				GUI::line %= f->getListSize() + 1;
			}

			// ============= DOWN =============
			if (GetKeyState(VK_DOWN) & 0x8000) {
				GUI::line++;
				GUI::line %= f->getListSize() + 1;
			}

			// ============= BACK =============
			if (GetKeyState(0x08) & 0x8000) {
				while ((GetAsyncKeyState(VK_BACK) & 0x8000)) {};

				back = true;
				GUI::reset();
			}

			// ============= EXIT =============
			if (GetKeyState(0x1B) & 0x8000) {
				GUI::esc = true;
			}

			// ============= PASSWORD =============
			if (GetKeyState(0x50) & 0x8000) {
				while ((GetAsyncKeyState(0x50) & 0x8000)) {};

				this->setPassword(f->getEntryInList(GUI::line - 1));
			}

			// ========== DELETE A FILE/FOLDER ==========
			if (GetKeyState(0x44) & 0x8000) {
				while ((GetAsyncKeyState(0x44) & 0x8000)) {};

				if (GUI::line != 0) {
					this->del(f->getEntryInList(GUI::line - 1), f);
				}
			}

			// Refresh menu
			if (!GUI::esc) {
				updateMenu(f);
			}
		}


		if (back || GUI::esc) break;
	}

}

void Volume::updateMenu(Entry* entry)
{
	clrscr();

	setColor(0, 10);
	cout << " Path ";

	setColor(10, 0);
	cout << " " << entry->getPath(); printSpace(123 - entry->getPath().size() - 7);

	setColor(0, 7);
	cout << " Name"; printSpace(42); cout << " | "; printSpace(16); cout << "Size | Type"; printSpace(6); cout << " | Modified"; printSpace(16); cout << " |  Password " << endl;
	setColor(15, 0);

	entry->show(GUI::line);
}

void Volume::enterFolder(Entry* parent, bool& back)
{
	if (GUI::line == 0) {
		back = true;
		GUI::reset();
		return;
	}

	string pw;

	Entry* f = parent->getEntryInList(GUI::line - 1);

	if (!f->isFolder()) return;

	if (f->isLocked()) {
		clrscr();
		GUI::clearBackground();

		pw = GUI::enterPassword();

		if (f->checkPassword(pw)) {
			this->navigate(f);
		}
		else {
			setColor(4, 0);
			cout << "Error: Invalid password. Access folder denied. ";
			system("pause");
			setColor(15, 0);
		}
	}
	else {
		this->navigate(f);
	}
}

void Volume::setPassword(Entry* f)
{
	if (GUI::line == 0) return; // Case parent folder

	clrscr();
	GUI::clearBackground();

	string pw = GUI::enterPassword();

	if (f->isLocked()) {
		if (f->checkPassword(pw)) f->resetPassword();
		else {
			setColor(4, 0);
			cout << "Error: Invalid password. Reset pasword denied. ";
			system("pause");
			setColor(15, 0);
		}
	}
	else {
		f->setPassword(pw);
	}
}

void Volume::del(Entry* entry, Entry* parent)
{
	// Step 1: Delete this entry on File
	size_t newEndPosOfVolumeFile = 0;
	fstream file(this->Path);
	if (file.is_open()) {
		file.clear();

		// Step 1.1: Data Field
		size_t const BLOCK_SIZE = 4096;	// byte
		uint8_t subData[BLOCK_SIZE];

		entry->seekToHeadOfData(file);
		size_t startWrite = (size_t)file.tellg();

		entry->seekToEndOfData(file);
		size_t startRead = (size_t)file.tellg();

		this->VolumeInfo.seekToHeadOfEntryTable(file);
		size_t endDataField = (size_t)file.tellg();

		size_t shiftingDataSize = endDataField - startRead;

		for (size_t i = 0; i < shiftingDataSize / BLOCK_SIZE; ++i) {
			file.seekg(startRead);
			file.read((char*)subData, BLOCK_SIZE);
			startRead += BLOCK_SIZE;

			file.seekg(startWrite);
			file.write((char*)subData, BLOCK_SIZE);
			startWrite += BLOCK_SIZE;
		}
		shiftingDataSize %= BLOCK_SIZE;		// remain
		file.seekg(startRead);
		file.read((char*)subData, shiftingDataSize);
		file.seekg(startWrite);
		file.write((char*)subData, shiftingDataSize);

		// Step 1.2: Entry Table
		this->EntryTable.updateAfterDel(entry);
		this->EntryTable.write(file);

		// Step 1.3: Volume Info
		this->VolumeInfo.updateAfterDel(entry);
		this->VolumeInfo.write(file);

		newEndPosOfVolumeFile = file.tellg();
	}
	file.close();

	// Step 1.4: Resize this Volume File
	LPTSTR lpfname = new TCHAR[this->Path.length() + 1];
	for (size_t i = 0; i < this->Path.length(); ++i) {
		lpfname[i] = (CHAR)this->Path[i];
	}
	lpfname[this->Path.length()] = '\0';

	HANDLE file_1 = CreateFile(
		lpfname,
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD dwErr = GetLastError();
	if (dwErr > 0) {
		cout << "Error: " << dwErr << endl;
		throw;
	}

	SetFilePointer(file_1, newEndPosOfVolumeFile, 0, FILE_BEGIN);
	SetEndOfFile(file_1);
	CloseHandle(file_1);

	// Step 2: Find and delete all sub-entries of this entry (Recursively)
	vector<Entry*> subEntryList = entry->getSubEntryList();
	for (Entry* subEntry : subEntryList) {
		this->del(subEntry, entry);
	}

	// Step 3: Delete this entry on RAM
	parent->del(entry);
}

void Volume::seekToHeadOfVolumeInfo(fstream& file) const
{
	file.seekg(0 - (int)sizeof(VolumeInfo), ios_base::end);
}

void Volume::seekToHeadOfEntryTable(fstream& file) const
{
	this->VolumeInfo.seekToHeadOfEntryTable(file);
}

