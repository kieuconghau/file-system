#pragma once

#include "GUI.h"
#include "VolumeInfo.h"
#include "EntryTable.h"

#include <Windows.h>

class Volume
{
public:
	Volume();
	Volume(string const& volumeFilePath);
	~Volume();

	bool isVolumeFile();
	bool create();
	void open();

	string getPath() const;

private:
	void initialize(string const& volumeFilePath);

	void seekToHeadOfVolumeInfo(fstream& file) const;
	void seekToHeadOfEntryTable(fstream& file) const;

	void performFunctions();
	void navigate(Entry* entry);
	void updateMenu(Entry* entry);
	void enterFolder(Entry* parent, bool& back);

	void setPassword(Entry* f);
	void writePasswordChange();
	void deleteOnVolume(Entry* f);
	bool del(Entry* entry, Entry* parent);

	void resize(size_t const& size);

private:
	string		Path;
	VolumeInfo	VolumeInfo;
	EntryTable	EntryTable;
};

