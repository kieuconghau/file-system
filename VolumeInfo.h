#pragma once
#include <iostream>
#include <fstream>
using namespace std;

class VolumeInfo
{
public:
	VolumeInfo();
	~VolumeInfo() = default;

	void write(ofstream& file) const;
	void read(ifstream& file) const;
	bool checkSignature(ifstream& file) const;
	
	void seekToHeadOfEntryTable(ifstream& file) const;
	bool isEndOfEntryTable(ifstream& file) const;

private:
	/*==========*/
	uint32_t Signature;
	uint32_t SizeEntryTable;
	uint32_t OffsetEntryTable;
	/*==========*/
};

