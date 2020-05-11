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

	/* ============== BUU WRITE THIS ============== */
	void display(bool selected);
	void displayParent(bool selected);
	void show(int selected);
	int getListSize() { return EntryList.size(); };
	Entry* getEntryInList(int i) { return EntryList[i]; };
	/* ============== BUU WRITE THIS ============== */

private:
	vector<Entry*>	EntryList;
};

