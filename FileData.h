#pragma once

#include <iostream>
#include <fstream>
using namespace std;

class FileData
{
public:
	FileData();
	FileData(size_t const& fileSize);
	FileData(FileData const& fileData);
	FileData& operator=(FileData const& fileData);
	~FileData();

	void read(ifstream& fileStream);
	void write(fstream& volumeStream);

private:
	uint32_t FileSize;
	char* Data;
};

