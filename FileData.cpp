#include "FileData.h"

FileData::FileData()
{
	this->FileSize = 0;
	this->Data = nullptr;
}

FileData::FileData(size_t const& fileSize)
{
	this->FileSize = fileSize;
	this->Data = new char[this->FileSize];
}

FileData::FileData(FileData const& fileData)
{
	this->FileSize = fileData.FileSize;
	this->Data = new char[this->FileSize];
	for (size_t i = 0; i < this->FileSize; ++i) {
		this->Data[i] = fileData.Data[i];
	}
}

FileData& FileData::operator=(FileData const& fileData)
{
	if (this != &fileData) {
		this->FileSize = fileData.FileSize;
		this->Data = new char[this->FileSize];
		for (size_t i = 0; i < this->FileSize; ++i) {
			this->Data[i] = fileData.Data[i];
		}
	}

	return *this;
}

FileData::~FileData()
{
	if (this->Data != nullptr) {
		delete this->Data;
		this->Data = nullptr;
	}
}

void FileData::read(ifstream& fileStream)
{
	fileStream.read(this->Data, this->FileSize);
}

void FileData::write(fstream& volumeStream)
{
	volumeStream.write(this->Data, this->FileSize);
}
