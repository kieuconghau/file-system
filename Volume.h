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

	void del(Entry* entry, Entry const* parent);

private:
	void seekToHeadOfVolumeInfo(fstream& file) const;
	void seekToHeadOfEntryTable(fstream& file) const;

private:
	string		Path;
	VolumeInfo	VolumeInfo;
	EntryTable	EntryTable;
};

