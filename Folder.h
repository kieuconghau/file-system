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
	bool hasChildWithTheSameName(Entry const& entry) const override;

	/* ============== BUU WRITE THIS ============== */
	void display(bool selected);
	void displayParent(bool selected);
	void show(int selected);
	int getListSize() { return EntryList.size(); };
	Entry* getEntryInList(int i) { return EntryList[i]; };
	/* ============== BUU WRITE THIS ============== */

private:
	vector<Entry*> EntryList;
};

