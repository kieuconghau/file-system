#include "Entry.h"

Entry::Entry()
{
	this->ModifiedTime	= 0;
	this->ModifiedDate	= 0;
	this->Size			= 0;
	this->PathLen		= 0;
	this->PasswordLen	= 0;
	this->OffsetData	= 0;
	this->Path			= "";
	this->Password		= "";
	this->Name			= "";
}

Entry::Entry(Entry const& entry)
{
	*this = entry;
}

Entry::Entry(uint32_t const& size, string const& path, string const& password) : Entry()
{
	this->Size = size;
	this->Path = path;
	this->Password = password;
	this->PathLen = path.length();
	this->PasswordLen = password.length();
}

void Entry::read(ifstream& file)
{
	file.read((char*)&this->ModifiedTime, sizeof(this->ModifiedTime));
	file.read((char*)&this->ModifiedDate, sizeof(this->ModifiedDate));
	file.read((char*)&this->Size, sizeof(this->Size));
	file.read((char*)&this->PathLen, sizeof(this->PathLen));
	file.read((char*)&this->PasswordLen, sizeof(this->PasswordLen));
	file.read((char*)&this->OffsetData, sizeof(this->OffsetData));

	this->Path.resize(this->PathLen);
	file.read((char*)this->Path.c_str(), this->PathLen);

	this->Password.resize(this->PasswordLen);
	file.read((char*)this->Password.c_str(), this->PasswordLen);

	this->splitPath();
}

bool Entry::isFolder() const
{
	return Path.back() == SLASH;
}

bool Entry::isLocked() const
{
	return this->PathLen != 0;
}

bool Entry::hasName(string const& name) const
{
	return name == this->Name;
}

vector<string> Entry::getAncestorNameList() const
{
	return this->AncestorNameList;
}

string Entry::getPath() const
{
	return this->Path;
}

Entry* Entry::findParent(vector<string>& ancestorNameList) const
{
	return nullptr;
}

void Entry::add(Entry const& entry) {}

void Entry::write(ofstream& file) const
{
	file.write((char*)&this->ModifiedTime, sizeof(this->ModifiedTime));
	file.write((char*)&this->ModifiedDate, sizeof(this->ModifiedDate));
	file.write((char*)&this->Size, sizeof(this->Size));
	file.write((char*)&this->PathLen, sizeof(this->PathLen));
	file.write((char*)&this->PasswordLen, sizeof(this->PasswordLen));
	file.write((char*)&this->OffsetData, sizeof(this->OffsetData));
	file.write((char*)this->Path.c_str(), this->PathLen);
	file.write((char*)this->Password.c_str(), this->PasswordLen);
}

void Entry::splitPath()
{
	string tempPath = this->Path;
	if (!this->isFolder()) {
		tempPath += SLASH;
	}

	size_t slashIndex;
	for (size_t i = 0; i < tempPath.length(); i = slashIndex + 1) {
		slashIndex = tempPath.find(SLASH, i);
		this->AncestorNameList.push_back(tempPath.substr(i, slashIndex - i));
	}

	if (this->AncestorNameList.size() != 0) {
		this->Name = this->AncestorNameList.back();
		this->AncestorNameList.pop_back();
		this->AncestorNameList.shrink_to_fit();
	}
}
