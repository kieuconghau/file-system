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
  
	void open();
  
	void read(fstream& file, VolumeInfo const& volumeInfo);
	void write(fstream& file) const;

	void add(Entry const& entry);
	
	void updateAfterDel(Entry const* entry);

private:
	Entry* Root;
	vector<Entry*> EntryList;
};

