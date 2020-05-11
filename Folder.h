#pragma once

#include "Entry.h"
#include "File.h"

class Folder : public Entry
{
public:
	Folder() = default;
	Folder(Entry const& entry);
	~Folder();

	Entry* findParent(vector<string>& ancestorNameList) const override;
	Entry* add(Entry const& tempEntry) override;
	void del() override;

private:
	vector<Entry*>	EntryList;
};

