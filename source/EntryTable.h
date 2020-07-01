#pragma once
#include "VolumeInfo.h"
#include "Entry.h"
#include "Folder.h"
#include "GUI.h"

class EntryTable
{
	friend class Volume;

public:
	EntryTable();
	~EntryTable();
  
	void read(fstream& file, VolumeInfo const& volumeInfo);
	void write(fstream& file) const;

	void add(Entry const& entry);
	
	void updateAfterDel(Entry const* entry);

private:
	Entry* Root;
	vector<Entry*> EntryList;
};

