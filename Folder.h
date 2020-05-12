#pragma once

#include "Entry.h"
#include "File.h"

class Folder : public Entry
{
public:
	Folder() = default;
	Folder(Entry const& entry);
	~Folder();

	Entry* add(Entry const& tempEntry) override;
	void del(Entry* entry) override;
	vector<Entry*> getSubEntryList() const override;

private:
	vector<Entry*> EntryList;
};

