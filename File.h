#pragma once

#include "Entry.h"

class File : public Entry
{
public:
	File() = default;
	File(Entry const& entry);
	~File();

	Entry* add(Entry const& entry) override;
	void del(Entry* entry) override;
	vector<Entry*> getSubEntryList() const override;
};

