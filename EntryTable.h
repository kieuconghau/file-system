#pragma once

#include "VolumeInfo.h"
#include "Entry.h"
#include "Folder.h"

class EntryTable
{
public:
	EntryTable();
	~EntryTable();

	void read(ifstream& file, VolumeInfo const& volumeInfo) const;

private:
	Entry* Root;
};

