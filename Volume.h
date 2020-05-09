#pragma once

#include "VolumeInfo.h"
#include "EntryTable.h"

class Volume
{
public:
	Volume();
	~Volume() = default;

	void create(string const& volumeFilePath);
	void open(string const& volumeFilePath);
	bool isVolumeFile(string const& volumeFilePath);

private:
	void seekToHeadOfVolumeInfo(ifstream& file) const;
	void seekToHeadOfEntryTable(ifstream& file) const;

private:
	string		Path;
	VolumeInfo	VolumeInfo;
	EntryTable	EntryTable;
};

