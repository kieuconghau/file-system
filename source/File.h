#pragma once

#include "Entry.h"

class File : public Entry
{
public:
	File() = default;
	File(Entry const& entry);
	~File();
	
	/* ============== BUU WRITE THIS ============== */
	void display(bool selected) override;
	/* ============== BUU WRITE THIS ============== */
  
	Entry* add(Entry const& entry) override;
	void del(Entry* entry) override;
	vector<Entry*> getSubEntryList() const override;
	bool hasChildWithTheSameName(Entry const& entry) const override;
};

