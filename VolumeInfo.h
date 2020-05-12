#pragma once
#include <iostream>
#include <fstream>
using namespace std;

#include "Entry.h"

class VolumeInfo
{
public:
	VolumeInfo();
	~VolumeInfo();

	void write(fstream& file) const;
	void read(fstream& file) const;
	bool checkSignature(fstream& file) const;
	
	void seekToHeadOfEntryTable(fstream& file) const;
	bool isEndOfEntryTable(fstream& file) const;

	void updateAfterDel(Entry const* entry);

private:
	/*==========*/
	uint32_t Signature;
	uint32_t SizeEntryTable;
	uint32_t OffsetEntryTable;
	/*==========*/
};

