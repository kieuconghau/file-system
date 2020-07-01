#include "Entry.h"

Entry::Entry()
{
	this->ModifiedTime	= 0;
	this->ModifiedDate	= 0;
	this->SizeData		= 0;
	this->PathLen		= 0;
	this->PasswordLen	= 0;
	this->OffsetData	= 0;
	this->Path			= "";
	this->Password		= "";
	this->Name			= "";
	
	this->FullPathOutside = "";
	this->IsFolder = false;
}

Entry::Entry(Entry const& entry)
{
	*this = entry;
}

Entry::~Entry() {}

void Entry::read(fstream& file)
{
	file.read((char*)&this->ModifiedTime, sizeof(this->ModifiedTime));
	file.read((char*)&this->ModifiedDate, sizeof(this->ModifiedDate));
	file.read((char*)&this->SizeData, sizeof(this->SizeData));
	file.read((char*)&this->PathLen, sizeof(this->PathLen));
	file.read((char*)&this->PasswordLen, sizeof(this->PasswordLen));
	file.read((char*)&this->OffsetData, sizeof(this->OffsetData));

	this->Path.resize(this->PathLen);
	file.read((char*)this->Path.c_str(), this->PathLen);

	this->Password.resize(this->PasswordLen);
	file.read((char*)this->Password.c_str(), this->PasswordLen);

	this->initializeName();
}

void Entry::write(fstream& file) const
{
	file.write((char*)&this->ModifiedTime, sizeof(this->ModifiedTime));
	file.write((char*)&this->ModifiedDate, sizeof(this->ModifiedDate));
	file.write((char*)&this->SizeData, sizeof(this->SizeData));
	file.write((char*)&this->PathLen, sizeof(this->PathLen));
	file.write((char*)&this->PasswordLen, sizeof(this->PasswordLen));
	file.write((char*)&this->OffsetData, sizeof(this->OffsetData));
	file.write((char*)this->Path.c_str(), this->PathLen);
	file.write((char*)this->Password.c_str(), this->PasswordLen);
}

bool Entry::isFolder() const
{
	if (this->Path == "") {
		throw "Logic Error";
	}
	return this->Path.back() == SLASH;
}

bool Entry::isLocked() const
{
	return this->PasswordLen != 0;
}

bool Entry::hasName(string const& name) const
{
	return name == this->Name;
}

bool Entry::hasParent(Entry const* parent) const
{
	string parentPath = parent->getPath();

	if (parentPath.length() >= this->PathLen) {
		return false;
	}

	size_t i = 0;

	while (i < parentPath.length()) {
		if (parentPath[i] != this->Path[i]) {
			return false;
		}
		++i;
	}

	while (i < (size_t)(this->PathLen - this->isFolder())) {
		if (this->Path[i] == SLASH) {
			return false;
		}
		++i;
	}

	return true;
}

string Entry::getPath() const
{
	return this->Path;
}

uint32_t Entry::getSizeData() const
{
	return this->SizeData;
}

uint32_t Entry::getSize() const
{
	return sizeof(this->ModifiedTime) + sizeof(this->ModifiedDate)
		+ sizeof(this->SizeData) + sizeof(this->PathLen)
		+ sizeof(this->PasswordLen) + sizeof(this->OffsetData)
		+ this->Path.length() + this->Password.length();
}

uint16_t Entry::getPasswordLen() const
{
	return this->PasswordLen;
}

string Entry::getFullPathOutside() const
{
	return this->FullPathOutside;
}

bool Entry::getIsFolder() const
{
	return this->IsFolder;
}

Entry* Entry::add(Entry const& entry) { return nullptr; }

void Entry::write(ofstream& file) const
{
	file.write((char*)&this->ModifiedTime, sizeof(this->ModifiedTime));
	file.write((char*)&this->ModifiedDate, sizeof(this->ModifiedDate));
	file.write((char*)&this->SizeData, sizeof(this->SizeData));
	file.write((char*)&this->PathLen, sizeof(this->PathLen));
	file.write((char*)&this->PasswordLen, sizeof(this->PasswordLen));
	file.write((char*)&this->OffsetData, sizeof(this->OffsetData));
	file.write((char*)this->Path.c_str(), this->PathLen);
	file.write((char*)this->Password.c_str(), this->PasswordLen);
}

void Entry::del(Entry* entry) {}

vector<Entry*> Entry::getSubEntryList() const
{
	return vector<Entry*>();
}

bool Entry::hasChildWithTheSameName(Entry const& entry) const
{
	return true;
}

void Entry::seekToHeadOfData_g(fstream& file) const
{
	file.seekg((uint64_t)this->OffsetData);
}

void Entry::seekToHeadOfData_p(fstream& file) const
{
	file.seekp((uint64_t)this->OffsetData);
}

void Entry::seekToEndOfData_g(fstream& file) const
{
	file.seekg((uint64_t)this->OffsetData + (uint64_t)this->SizeData);
}

void Entry::seekToEndOfData_p(fstream& file) const
{
	file.seekp((uint64_t)this->OffsetData + (uint64_t)this->SizeData);
}

void Entry::getFileInfoAndConvertToEntry(_WIN32_FIND_DATAA ffd,
	string file_path, string file_name_in_volume,
	uint64_t& insert_pos)
{
	// File last modification date and time.
	FileTimeToDosDateTime(&ffd.ftLastWriteTime, &this->ModifiedDate,
		&this->ModifiedTime);

	// File size.
	this->SizeData = ffd.nFileSizeLow;

	// File name length.
	this->PathLen = file_name_in_volume.length();

	// File password length.
	this->PasswordLen = 0;

	// File offset.
	this->OffsetData = insert_pos;
	insert_pos += this->SizeData;

	// File name.
	this->Path = file_name_in_volume;

	this->FullPathOutside = file_path;

	if (ffd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY) {
		this->IsFolder = true;
	}
	else {
		this->IsFolder = false;
	}
}

void Entry::standardizeAfterImport(Entry* parent)
{
	this->standardizePath();
	this->updatePathAfterImport(parent);
	this->initializeName();
}

void Entry::initializeName()
{
	size_t startPosOfName = 0;
	for (int i = this->PathLen - 1 - this->isFolder(); i >= 0; --i) {
		if (this->Path[i] == SLASH) {
			startPosOfName = i + 1;
			break;
		}
	}
	this->Name = this->Path.substr(startPosOfName, this->PathLen - startPosOfName - this->isFolder());
}

void Entry::standardizePath()
{
	string tempPath = "";

	for (size_t i = 0; i < this->PathLen; ++i) {
		if (this->Path[i] == '\\') {
			tempPath += '/';
		}
		else {
			tempPath += this->Path[i];
		}
	}

	this->Path = tempPath;
	this->PathLen = tempPath.length();
}

void Entry::updatePathAfterImport(Entry* parent)
{
	this->Path = parent->getPath() + this->Path;
	this->PathLen = this->Path.length();
}

void Entry::display(bool selected) {
	if (selected) setColor(15, 1);

	//Name
	cout << " " << Name;
	printSpace(49 - Name.length());

	//Size
	if (!isFolder()) {
		string s = numCommas(this->SizeData);
		printSpace(20 - s.length());
		cout << s << "   ";
	}
	else {
		printSpace(23);
	}
	

	//Type
	gotoXY(whereX() + 10, whereY());

	//Modified
	cout << "   ";
	this->displayModDate();
	cout << " ";
	this->displayModTime();
	printSpace(27 - 19);
	
	//Password
	if (isLocked()) {
		printSpace(10 - 4);
		cout << "[ON]";
	}
	else {
		printSpace(10 - 5);
		cout << "[OFF]";
	}
	cout << endl;

	if (selected) setColor(15, 0);
}

void Entry::setPassword(string pw) {
	SHA256 sha256;
	this->Password = sha256(addPepper(addSalt(pw)));
	this->PasswordLen = Password.length();
}

void Entry::resetPassword() {
	this->Password = "";
	this->PasswordLen = 0;
}

bool Entry::checkPassword(string pw) {
	SHA256 sha256;
	uint8_t salt[3] = { 0x4E , 0x48 ,0x54 };

	// Check 1st XOR
	for (int i = 0; i < pw.length(); i++) {
		GUI::checkPassword(i);

		string toTEST_0 = pw;
		toTEST_0[i] = toTEST_0[i] ^ salt[0];


		// Check 2nd XOR
		for (int j = 0; j < pw.length(); j++) {
			string toTEST_1 = toTEST_0;
			toTEST_1[j] = toTEST_1[j] ^ salt[1];


			// Check 3rd XOR
			for (int u = 0; u < pw.length(); u++) {
				string toTEST_2 = toTEST_1;
				toTEST_2[u] = toTEST_2[u] ^ salt[2];


				// Hash
				uint8_t v = 0;
				while (true) {

					string toSHA256 = toTEST_2 + (char)v;
					toSHA256 = sha256(toSHA256);

					// Check Hashed string with database
					if (this->Password.compare(toSHA256) == 0) return true;

					if (v == 255) break;
					else v++;
				}
			}
		}
	}

	return false;
}

void Entry::displayModDate() {
	uint8_t d, m, y;
	d = m = y = 0;
	string date;

	// Day
	d = this->ModifiedDate & 31;
	if (d < 10) {
		cout << "0" << (int)d << "/";
	}
	else {
		cout << (int)d << "/";
	}

	// Month
	m = (this->ModifiedDate >> 5) & 15;
	if (m < 10) {
		cout << "0" << (int)m << "/";
	}
	else {
		cout << (int)m << "/";
	}

	// Year
	y = (this->ModifiedDate >> 9);
	cout << ((int)y + 1980);

}

void Entry::displayModTime() {
	uint8_t hr, m, s;

	// Hour
	hr = (this->ModifiedTime >> 11);
	if (hr < 10) {
		cout << "0" << (int)hr << ":";
	}
	else {
		cout << (int)hr << ":";
	}

	// Min
	m = (this->ModifiedTime >> 5) & 63;
	if (m < 10) {
		cout << "0" << (int)m << ":";
	}
	else {
		cout << (int)m << ":";
	}

	// Sec
	s = (this->ModifiedTime & 31) << 1;
	if (s < 10) {
		cout << "0" << (int)s;
	}
	else {
		cout << (int)s;
	}

}