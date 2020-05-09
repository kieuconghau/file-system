#pragma once

#include "Entry.h"
#include "File.h"

class Folder : public Entry
{
public:
	Folder();
	Folder(Entry const& entry, Folder* parent);
	~Folder();

	Entry* findParent(vector<string>& ancestorNameList) const;
	void add(Entry const& tempEntry);

private:
	Folder*			Parent;
	vector<Entry*>	EntryList;
};

