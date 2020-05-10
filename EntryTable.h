#pragma once
#include "GUI.h"
#include "VolumeInfo.h"
#include "Entry.h"
#include "Folder.h"

class EntryTable
{
public:
	EntryTable();
	~EntryTable();

	void read(ifstream& file, VolumeInfo const& volumeInfo) const;
	void open();

private:
	Entry* Root;
};

