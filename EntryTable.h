#pragma once

#include "VolumeInfo.h"
#include "Entry.h"
#include "Folder.h"

class EntryTable
{
public:
	EntryTable();
	~EntryTable();

	void read(fstream& file, VolumeInfo const& volumeInfo);

	void updateAfterDel(Entry const* entry);
	void write(fstream& file) const;

private:
	Entry* Root;
	vector<Entry*> EntryList;
};

