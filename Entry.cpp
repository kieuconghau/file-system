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
}

Entry::Entry(Entry const& entry)
{
	*this = entry;
}

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

	this->splitPath();
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
	return Path.back() == SLASH;
}

bool Entry::isLocked() const
{
	return this->PasswordLen != 0;
}

bool Entry::hasName(string const& name) const
{
	return name == this->Name;
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

Entry* Entry::findParent(vector<string>& ancestorNameList) const
{
	return nullptr;
}

Entry* Entry::add(Entry const& entry) { return nullptr; }

void Entry::del() {}

void Entry::seekToHeadOfData(fstream& file) const
{
	file.clear();
	file.seekg(this->OffsetData);
}

void Entry::seekToEndOfData(fstream& file) const
{
	file.clear();
	file.seekg((uint64_t)this->OffsetData + (uint64_t)this->SizeData);
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
