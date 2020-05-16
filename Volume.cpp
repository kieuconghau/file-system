#include "Volume.h"

string const Volume::Extension = ".nht";

Volume::Volume(string const& volumeFilePath) : VolumeInfo(), EntryTable()
{
	this->initialize(volumeFilePath);
}

Volume::~Volume() {}

bool Volume::create()
{
	// Check if this file exists, if yes, we cannot create a volume file with this name
	fstream tempFile(this->Path, ios_base::in | ios_base::binary);
	if (tempFile.is_open()) {
		tempFile.close();
		return false;
	}

	// Create a volume file
	fstream file(this->Path, ios_base::out | ios_base::binary);
	if (file.is_open()) {
		file.clear();
		this->VolumeInfo.write(file);
		file.close();
		return true;
	}

	return false;
}

void Volume::open()
{
	// Open a volume file, then read info of VolumeInfo and EntryTable
	fstream file(this->Path, ios_base::in | ios_base::binary);
	if (file.is_open()) {
		this->seekToHeadOfVolumeInfo_g(file);
		this->VolumeInfo.read(file);

		this->seekToHeadOfEntryTable_g(file);
		this->EntryTable.read(file, this->VolumeInfo);
		file.close();
	}

	// Show list of files/folders in this volume.
	// Perform all functions like: import, export, delete, set/reset password for a file/folder
	this->performFunctions();
}

bool Volume::isEmpty() const
{
	return this->VolumeInfo.isEmptyVolume();
}

void Volume::importGUI(Entry* parent)
{
	system("cls");

	setColor(COLOR::LIGHT_CYAN, COLOR::BLACK);
	GUI::printTextAtMid("===== IMPORT A FILE/FOLDER =====");
	cout << "\n\n";

	cout << "  Program: Input a path of a folder or a file that you want to import to this volume." << "\n\n";
	cout << "  User: ";

	setColor(COLOR::WHITE, COLOR::BLACK);
	string str;
	getline(cin, str);

	if (this->import(str, parent)) {
		setColor(COLOR::LIGHT_CYAN, COLOR::BLACK);
		cout << "\n\n" << "  Program: Import successfully." << "\n\n";
		cout << "  ";
		system("pause");
	}
	else {
		setColor(COLOR::LIGHT_RED, COLOR::BLACK);
		cout << "\n\n" << "  Program: Can not import this path into the volume." << "\n\n";
		cout << "           Maybe this path does not exist OR" << "\n\n";
		cout << "           This file or folder has the same name with the one in this volume." << "\n\n";
		cout << "           Please check again!" << "\n\n";
		cout << "  ";
		system("pause");
	}
}

bool Volume::import(string const& new_file_path, Entry* parent)
{
	fstream volumeStream(this->Path, ios_base::in | ios_base::out | ios_base::binary);

	if (!volumeStream.is_open()) {
		throw "Volume Path Error";
	}

	// Step 1: We'll get and store the info and data of the new file(s)
	// into our program.

	// Step 1.1: Get the info.

	// If the file we inputted is just a file (not a folder),
	// we'll get only the info of that file. But if the file
	// we inputted in is in fact a folder, then:

	// We'll use "level order tree traversal" to travel to each
	// and every file or folder in the "directory tree"
	// rooted at the file we inputted in.
	// Each time we arrive at a file or folder in the tree,
	// we'll get the info of that file or folder.

	vector<Entry> file_entry_vector;

	Entry file_entry;
	_WIN32_FIND_DATAA ffd;

	// We'll use a variable to keep track of the position
	// where data of a new file is going to be inserted.
	// At first its value will be the original offset
	// of the Entry Table (which is also the end position
	// of the Data Area).
	// Every time a file is found and an entry is created,
	// the value of this variable will be used to fill in
	// the "file offset" field of the entry. After that,
	// the value of this variable will be updated.

	uint32_t insert_pos;
	if (this->isEmpty()) {
		insert_pos = 0;
	}
	else {
		insert_pos = this->VolumeInfo.getEntryTableOffset();
	}

	HANDLE hFile = FindFirstFileA(new_file_path.c_str(), &ffd);
	if (hFile == INVALID_HANDLE_VALUE) {
		volumeStream.close();
		return false;
	}

	// If the current file is actually a folder.
	if (ffd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
		queue<string> folder_path_queue;

		const string root_folder_path = new_file_path;
		const string root_folder_name = ffd.cFileName;
		string parent_folder_path;
		string file_name_in_volume;

		file_name_in_volume = root_folder_name;
		file_name_in_volume += "\\";

		file_entry.getFileInfoAndConvertToEntry(ffd,
			root_folder_path, file_name_in_volume, insert_pos);
		file_entry_vector.push_back(file_entry);

		folder_path_queue.push(root_folder_path);

		Entry root = file_entry_vector[0];
		root.standardizeAfterImport(parent);
		if (parent->hasChildWithTheSameName(root)) {
			volumeStream.close();
			return false;
		}

		while (folder_path_queue.empty() == false) {
			parent_folder_path = folder_path_queue.front() + "\\*";
			folder_path_queue.pop();

			// Every folder always contains two special subdirectories:
			// "." (which refers to the current directory)
			// and ".." (which refers to the parent directory).
			// For some reason, the file finding function in WinAPI
			// when searching for files in a folder
			// will always find these two special directories first.
			// So, when we've found these two directories, we have to
			// ignore them.

			// Find the "." directory, and then ignore it.
			hFile = FindFirstFileA(parent_folder_path.c_str(), &ffd);
			if (hFile == INVALID_HANDLE_VALUE) continue;

			// Find the ".." directory, and then ignore it.
			if (FindNextFileA(hFile, &ffd) == 0) continue;

			// Now we can find the first REAL file in the current folder.

			// If the finding function can't find any file
			// (in other words, this folder is empty),
			// we can continue with the next folder in the queue.
			if (FindNextFileA(hFile, &ffd) == 0) continue;

			do {
				Entry temp_entry;
				string file_path = parent_folder_path.substr(
					0, parent_folder_path.length() - 1) + ffd.cFileName;

				file_name_in_volume =
					parent_folder_path.substr(
						root_folder_path.length() - root_folder_name.length(),
						parent_folder_path.length()
						- root_folder_path.length()
						+ root_folder_name.length()
						- 1)
					+ ffd.cFileName;

				if (ffd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
					file_name_in_volume += "\\";
				}

				temp_entry.getFileInfoAndConvertToEntry(ffd,
					file_path, file_name_in_volume, insert_pos);
				file_entry_vector.push_back(temp_entry);

				// If the current file is a folder.
				if (ffd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
					// Get the parent folder's path, remove the asterisk
					// at the tail, append the current folder's name,
					// and push the whole path into the queue.
					folder_path_queue.push(
						parent_folder_path.substr(0,
							parent_folder_path.length() - 1)
						+ ffd.cFileName);
				}

			} while (FindNextFileA(hFile, &ffd) != 0);

		}
	}
	else {
		file_entry.getFileInfoAndConvertToEntry(ffd, new_file_path,
			ffd.cFileName, insert_pos);
		file_entry_vector.push_back(file_entry);
	}

	FindClose(hFile);

	// Convert to standardized entry
	for (size_t i = 0; i < file_entry_vector.size(); ++i) {
		file_entry_vector[i].standardizeAfterImport(parent);
	}

	// Step 1.2: Get the data

	// Now we'll get the data of the files among the group
	// of files and folders in the directory tree we've just
	// finished traversing.
	// (Again, if we inputted in a normal file (not a folder)
	// then we'll just get the data of that file only.)

	vector<FileData> file_data_vector;

	ifstream file_stream;

	for (size_t i = 0; i < file_entry_vector.size(); i++) {

		// We only get the data of the files which are not folders.
		if (file_entry_vector[i].getIsFolder() == false) {

			file_stream.open(file_entry_vector[i].getFullPathOutside(), ios::binary);
			if (file_stream.is_open() == false) continue;

			FileData file_data(file_entry_vector[i].getSizeData());
			
			file_data.read(file_stream);
			file_data_vector.push_back(file_data);

			file_stream.close();


		}
	}

	// All the info and data of the new file(s) have been prepared
	// and are waiting to be written to the volume.

	// Step 2: Write the info and data of the new file(s) into the volume.

	if (this->isEmpty() == false) {	// If the volume is not empty.

		// Set volume stream's pointer
		// to the beginning of Entry Table.
		this->seekToHeadOfEntryTable_p(volumeStream);
	}
	else {	// If the volume is empty

		// Set volume stream's pointer
		// to the beginning of the volume.
		volumeStream.seekp(0, ios::beg);
	}

	// Write a bundle of
	// "[Data of the new file(s)]
	// - [Entries of old files] (which is the content of the Entry Table
	// stored earlier)
	// - [Entry (or entries) of the new file(s)] (consisting of
	// the new file's (or new files') info)
	// - [Volume Info Area]"
	// and update the volume's info in the Volume Info Area.

	// Write data of the new file(s).
	for (size_t i = 0; i < file_data_vector.size(); i++) {
		file_data_vector[i].write(volumeStream);
	}

	// Update the offset of Entry Table.
	this->VolumeInfo.updateOffsetEntryTable((uint32_t)volumeStream.tellp());

	// Add new entries to Entry Table (RAM)
	for (size_t i = 0; i < file_entry_vector.size(); ++i) {
		this->EntryTable.add(file_entry_vector[i]);
	}

	// Write updated Entry Table
	this->EntryTable.write(volumeStream);
	
	// Update the size of Entry Table
	this->VolumeInfo.updateSizeEntryTable((uint32_t)volumeStream.tellp());

	// Write the Volume Info Area
	this->VolumeInfo.write(volumeStream);

	volumeStream.close();

	return true;
}

void Volume::exportGUI(Entry* f) {
	system("cls");

	setColor(COLOR::LIGHT_CYAN, COLOR::BLACK);
	GUI::printTextAtMid("===== EXPORT A FILE/FOLDER =====");
	cout << "\n\n";

	cout << "  File/folder to be exported: ";

	setColor(COLOR::WHITE, COLOR::BLACK);
	cout << f->getPath() << endl;

	setColor(COLOR::LIGHT_CYAN, COLOR::BLACK);
	cout << "  Program: Input a path of a folder that you want to "
		<< "export this file/folder to." << "\n\n";
	cout << "  User: ";

	setColor(COLOR::WHITE, COLOR::BLACK);
	string str;
	getline(cin, str);

	if (this->exportFile(f, str)) {
		setColor(COLOR::LIGHT_CYAN, COLOR::BLACK);
		cout << "\n\n" << "  Program: Export successfully." << "\n\n";
		cout << "  ";
		system("pause");
	}
	else {
		setColor(COLOR::LIGHT_RED, COLOR::BLACK);
		cout << "\n\n" << "  Program: Can not export this file/folder to "
			<< "the specified path." << "\n\n";
		cout << "           Maybe the specified path does not exist." << "\n\n";
		cout << "           Please check again!" << "\n\n";
		cout << "  ";
		system("pause");
	}
}

bool Volume::exportFile(Entry* export_file_entry,
	string const& destination_path) {

	fstream volume_stream(this->Path, ios::in, ios_base::binary);
	if (volume_stream.is_open() == false) {
		throw "Volume Path Error";
	}

	// Check if the destination path exists.
	_WIN32_FIND_DATAA ffd;
	HANDLE hFile = FindFirstFileA(destination_path.c_str(), &ffd);
	if (hFile == INVALID_HANDLE_VALUE) {
		volume_stream.close();
		return false;
	}

	// We use "level order tree traversal" algorithm to travel to
	// each and every subfiles and subfolders of the inputted file
	// (if the inputted file is actually a folder).

	queue<Entry*> export_file_entry_queue;
	export_file_entry_queue.push(export_file_entry);

	while (export_file_entry_queue.empty() == false) {
		Entry* file_entry = export_file_entry_queue.front();
		export_file_entry_queue.pop();

		// If the file in question is a normal file (not a folder).
		if (file_entry->isFolder() == false) {
			string file_creation_path;
			file_creation_path = destination_path + "\\"
				+ file_entry->getPath().substr(
					export_file_entry->getPath().length()
					- (export_file_entry->getName().length() + 1),
					file_entry->getPath().length()
					- (export_file_entry->getPath().length()
						- (export_file_entry->getName().length() + 1)
						)
				);

			ofstream export_file_stream;
			export_file_stream.open(file_creation_path, ios::binary);

			if (export_file_stream.is_open() == true) {
				const int BUFFER_SIZE = 4096;
				char buffer[BUFFER_SIZE];

				file_entry->seekToHeadOfData_g(volume_stream);
				for (int i = 0; i < file_entry->getSizeData() / BUFFER_SIZE;
					i++) {
					volume_stream.read(buffer, BUFFER_SIZE);
					export_file_stream.write(buffer, BUFFER_SIZE);
				}
				volume_stream.read(buffer,
					file_entry->getSizeData() % BUFFER_SIZE);
				export_file_stream.write(buffer,
					file_entry->getSizeData() % BUFFER_SIZE);

				export_file_stream.close();
			}
		}

		// If the file in question is actually a folder.
		else {
			string folder_creation_path;
			folder_creation_path = destination_path + "\\"
				+ file_entry->getPath().substr(
					export_file_entry->getPath().length()
					- (export_file_entry->getName().length() + 1),
					file_entry->getPath().length()
					- (export_file_entry->getPath().length()
						- (export_file_entry->getName().length() + 1)
						) - 1
				);

			// If the folder is successfully created.
			if (CreateDirectoryA(
				folder_creation_path.c_str(), NULL) != 0) {
				// Push the entries of subfiles into the queue.
				vector<Entry*> sub_entry_list = file_entry->getSubEntryList();
				for (size_t i = 0; i < sub_entry_list.size(); i++) {
					export_file_entry_queue.push(sub_entry_list[i]);
				}
			}
		}
	}

	volume_stream.close();
	return true;
}

string Volume::getPath() const
{
	return this->Path;
}

bool Volume::isVolumeFile()
{
	// Open file and check if this file is a volume file
	bool isVF = false;

	fstream file(this->Path, ios_base::in | ios_base::binary);
	if (file.is_open()) {
		file.clear();
		this->seekToHeadOfVolumeInfo_g(file);
		this->VolumeInfo.read(file);
		isVF = this->VolumeInfo.checkSignature(file);
		file.close();
	}

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
			if (GetKeyState(0x0D) & 0x8000) {	// ENTER
				while ((GetAsyncKeyState(VK_RETURN) & 0x8000)) {};
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
			if (GetKeyState(0x08) & 0x8000) {	// BACKSPACE
				while ((GetAsyncKeyState(VK_BACK) & 0x8000)) {};

				back = true;
				GUI::reset();
			}

			// ============= EXIT =============
			if (GetKeyState(0x1B) & 0x8000) {	// ESC
				while ((GetAsyncKeyState(0x1B) & 0x8000)) {};

				GUI::esc = true;
			}

			// ============= PASSWORD =============
			if (GetKeyState(0x50) & 0x8000) {	// P
				while ((GetAsyncKeyState(0x50) & 0x8000)) {};

				if (GUI::line != 0) {
					this->setPassword(f->getEntryInList(GUI::line - 1));
				}
			}

			// ========== DELETE A FILE/FOLDER ==========
			if ((GetKeyState(0x44) & 0x8000) || (GetKeyState(0x2E) & 0x8000)) {	// D or DEL
				while ((GetKeyState(0x44) & 0x8000) || (GetKeyState(0x2E) & 0x8000)) {};

				this->deleteOnVolume(f);
			}

			// ========== IMPORT ==========
			if ((GetKeyState(0x49) & 0x8000)) {	// I
				while ((GetKeyState(0x49) & 0x8000)) {};

				this->importGUI(f);
			}

			// ========== EXPORT ==========
			if ((GetKeyState(0x45) & 0x8000)) {	// E
				while ((GetKeyState(0x45) & 0x8000)) {};

				this->exportGUI(f->getEntryInList(GUI::line - 1));
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
	gotoXY(0, 1);
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
			setColor(COLOR::LIGHT_RED, COLOR::BLACK);
			gotoXY(0, 5); cout << "  Error: Invalid password. Access folder denied. ";
			gotoXY(0, 7); cout << "  "; system("pause");
			setColor(COLOR::WHITE, COLOR::BLACK);
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

	size_t oldPasswordLen = f->getPasswordLen();

	string pw = GUI::enterPassword();

	if (f->isLocked()) {
		if (f->checkPassword(pw)) {
			f->resetPassword();
		}
		else {
			setColor(COLOR::LIGHT_RED, COLOR::BLACK);
			gotoXY(0, 5);  cout << "  Error: Invalid password. Reset pasword denied. " << endl;
			gotoXY(0, 7); cout << "  ";  system("pause");
			setColor(COLOR::WHITE, COLOR::BLACK);
			return;
		}
	}
	else {
		f->setPassword(pw);
	}

	size_t newPasswordLen = f->getPasswordLen();

	this->VolumeInfo.updateAfterSetPassword(oldPasswordLen, newPasswordLen);
	this->writePasswordChange();
}

bool Volume::del(Entry* entry, Entry* parent)
{
	bool isTotallyDeleted = true;

	// Step 1: Find and delete all sub-entries of this entry (Recursively)
	vector<Entry*> subEntryList = entry->getSubEntryList();
	for (Entry* subEntry : subEntryList) {
		if (subEntry->isLocked()) {
			isTotallyDeleted = false;
			continue;
		}
		this->del(subEntry, entry);
	}

	// Step 2: Check if this entry still stores sub-entries, if yes, we can not delete this entry.
	if (entry->getListSize() != 0) {
		return isTotallyDeleted;
	}

	// Step 3: Delete this entry on File
	size_t newEndPosOfVolumeFile = 0;
	fstream file(this->Path, ios_base::in | ios_base::out | ios_base::binary);
	if (file.is_open()) {
		file.clear();

		// Step 3.1: Data Field
		size_t const BLOCK_SIZE = 4096;	// byte
		uint8_t subData[BLOCK_SIZE];

		entry->seekToHeadOfData_p(file);
		size_t startWrite = (size_t)file.tellp();

		entry->seekToEndOfData_g(file);
		size_t startRead = (size_t)file.tellg();

		this->VolumeInfo.seekToHeadOfEntryTable_g(file);
		size_t endDataField = (size_t)file.tellg();

		size_t shiftingDataSize = endDataField - startRead;

		for (size_t i = 0; i < shiftingDataSize / BLOCK_SIZE; ++i) {
			file.seekg(startRead);
			file.read((char*)subData, BLOCK_SIZE);
			startRead += BLOCK_SIZE;

			file.seekp(startWrite);
			file.write((char*)subData, BLOCK_SIZE);
			startWrite += BLOCK_SIZE;
		}
		shiftingDataSize %= BLOCK_SIZE;		// remain
		file.seekg(startRead);
		file.read((char*)subData, shiftingDataSize);
		file.seekp(startWrite);
		file.write((char*)subData, shiftingDataSize);

		// Step 3.2: Entry Table
		this->EntryTable.updateAfterDel(entry);
		this->EntryTable.write(file);

		// Step 3.3: Volume Info
		this->VolumeInfo.updateAfterDel(entry);
		this->VolumeInfo.write(file);

		newEndPosOfVolumeFile = file.tellp();
	}
	file.close();

	// Step 3.4: Resize this Volume File
	this->resize(newEndPosOfVolumeFile);

	// Step 4: Delete this entry on RAM
	parent->del(entry);

	return isTotallyDeleted;
}

void Volume::resize(size_t const& size)
{
	// Convert the path of this volume from string to LPTSTR
	LPTSTR lpfname = new TCHAR[this->Path.length() + 1];
	for (size_t i = 0; i < this->Path.length(); ++i) {
		lpfname[i] = (CHAR)this->Path[i];
	}
	lpfname[this->Path.length()] = '\0';

	// Open this volume file with the path name in LPTSTR type
	HANDLE file = CreateFile(
		lpfname,
		GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	// Check error after opening this volume file
	DWORD dwErr = GetLastError();
	if (dwErr > 0) {
		cout << "  Error: " << dwErr << endl;
		throw;
	}

	// Resize this volume file and close file
	SetFilePointer(file, size, 0, FILE_BEGIN);
	SetEndOfFile(file);
	CloseHandle(file);
}

void Volume::deleteOnVolume(Entry* f) {
	if (GUI::line == 0) return; // case Parent

	clrscr();
	GUI::clearBackground();

	string pw;

	Entry* parent = f;
	f = f->getEntryInList(GUI::line - 1);

	if (f->isLocked()) {
		pw = GUI::enterPassword();

		if (!f->checkPassword(pw)) {
			setColor(COLOR::LIGHT_RED, COLOR::BLACK);
			gotoXY(0, 5); cout << "  Error: Invalid password. Deletion denied. " << endl;
			gotoXY(0, 7); cout << "  "; system("pause");
			setColor(COLOR::WHITE, COLOR::BLACK);
			return;
		}

		gotoXY(0, 5);
	}

	string name = f->getName();
	setColor(COLOR::LIGHT_CYAN, COLOR::BLACK);
	cout << "  Program: Do you want to permanently DELETE '" << name << "'? " << endl;
	cout << endl;
	cout << "           Type DELETE to confirm | else it will cancel." << endl;
	cout << endl;
	cout << "  User:    ";
	setColor(COLOR::WHITE, COLOR::BLACK);  cin >> pw;
	cout << endl;
	setColor(COLOR::WHITE, COLOR::BLACK);

	if ((pw.compare("DELETE") == 0) || (pw.compare("delete") == 0)) {
	
		if (!this->del(f, parent)) {
			setColor(COLOR::LIGHT_RED, COLOR::BLACK);
			cout << "  Program: '" << name << "' can not be deleted totally." << endl;
			cout << endl;
			cout << "           Because this folder still stores some files or folders secured with password." << endl;
			cout << endl;
			cout << "           To delete '" << name << "' totally, make sure that all files or folders stored in this folder have no password." << endl;
		}
		else {
			setColor(COLOR::LIGHT_CYAN, COLOR::BLACK);
			cout << "  Program:  '" << name << "' is deleted successfully. " << endl;
		}
		cout << endl;
		cout << "  ";
		system("pause");
		setColor(COLOR::WHITE, COLOR::BLACK);
		GUI::reset();
	}
	else {
		setColor(COLOR::LIGHT_CYAN, COLOR::BLACK);
		cout << "  Program: Delete  '" << name << "' is canceled. " << endl;
		cout << endl;
		cout << "  ";
		system("pause");
		setColor(COLOR::WHITE, COLOR::BLACK);
	}

	
}

void Volume::initialize(string const& volumeFilePath)
{
	// Convert the path of this volume from string to LPTSTR
	LPTSTR lptstrVFP = new TCHAR[volumeFilePath.length() + 1];
	for (size_t i = 0; i < volumeFilePath.length(); ++i) {
		lptstrVFP[i] = (TCHAR)volumeFilePath[i];
	}
	lptstrVFP[volumeFilePath.length()] = '\0';

	// Get a full path (LPTSTR) for this volume file with the path in LPTSTR type
	LPTSTR tempPath = new TCHAR[MAX_PATH];
	GetFullPathName(lptstrVFP, MAX_PATH, tempPath, NULL);

	// Covert a full path name from LPTSTR to string
	for (size_t i = 0; i < MAX_PATH; ++i) {
		if (tempPath[i] == '\0') {
			break;
		}
		if (tempPath[i] == '\\') {
			this->Path += Entry::SLASH;
		}
		else {
			this->Path += tempPath[i];
		}
	}
}

void Volume::seekToHeadOfVolumeInfo_g(fstream& file) const
{
	file.seekg(0 - (int)sizeof(VolumeInfo), ios_base::end);
}

void Volume::seekToHeadOfVolumeInfo_p(fstream& file) const
{
	file.seekp(0 - (int)sizeof(VolumeInfo), ios_base::end);
}

void Volume::seekToHeadOfEntryTable_g(fstream& file) const
{
	this->VolumeInfo.seekToHeadOfEntryTable_g(file);
}

void Volume::seekToHeadOfEntryTable_p(fstream& file) const
{
	this->VolumeInfo.seekToHeadOfEntryTable_p(file);
}

void Volume::writePasswordChange() {
	fstream file(this->Path, ios_base::out | ios_base::binary);

	size_t newSize = 0;
	if (file.is_open()) {
		this->seekToHeadOfEntryTable_p(file);
		this->EntryTable.write(file);
		this->VolumeInfo.write(file);

		newSize = file.tellp();
		file.close();

		this->resize(newSize);
	}
}

