#pragma once

#include <iostream>
#include <fstream>
using namespace std;

#include "Entry.h"

class VolumeInfo
{
	friend class EntryTable;
public:
	VolumeInfo();
	~VolumeInfo() = default;

	void write(ofstream& file) const;
	void read(ifstream& file) const;
	bool checkSignature(ifstream& file) const;
	void seekToHeadOfEntryTable(ifstream& file) const;

	void write(ofstream& file, Entry const& entry);

private:
	/*==========*/
	uint32_t Signature;
	uint32_t SizeEntryTable;
	uint32_t OffsetEntryTable;
	/*==========*/
};

