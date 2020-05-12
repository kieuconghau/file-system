#pragma once

#include "GUI.h"
#include "VolumeInfo.h"
#include "EntryTable.h"

#include <Windows.h>

class Volume
{
public:
	Volume();
	~Volume();

	bool isVolumeFile(string const& volumeFilePath);
	void create(string const& volumeFilePath);
	void open(string const& volumeFilePath);

private:
	void seekToHeadOfVolumeInfo(fstream& file) const;
	void seekToHeadOfEntryTable(fstream& file) const;

	void performFunctions();
	void navigate(Entry* entry);
	void updateMenu(Entry* entry);
	void enterFolder(Entry* parent, bool& back);

	void setPassword(Entry* f);
	void writePasswordChange();
	void deleteOnVolume(Entry* f);
	void del(Entry* entry, Entry* parent);

private:
	string		Path;
	VolumeInfo	VolumeInfo;
	EntryTable	EntryTable;
};

