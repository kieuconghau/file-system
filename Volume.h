#pragma once

#include "VolumeInfo.h"
#include "EntryTable.h"

#include <Windows.h>

class Volume
{
public:
	Volume();
	~Volume();

	void create(string const& volumeFilePath);
	void open(string const& volumeFilePath);
	bool isVolumeFile(string const& volumeFilePath);

	void del(Entry* entry, Entry* parent);

private:
	void seekToHeadOfVolumeInfo(fstream& file) const;
	void seekToHeadOfEntryTable(fstream& file) const;

private:
	string		Path;
	VolumeInfo	VolumeInfo;
	EntryTable	EntryTable;
};

