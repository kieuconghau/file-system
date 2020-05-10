#include "EntryTable.h"

EntryTable::EntryTable()
{
	this->Root = new Folder();
}

EntryTable::~EntryTable()
{
	delete this->Root;
}

void EntryTable::read(ifstream& file, VolumeInfo const& volumeInfo) const
{
	while (!volumeInfo.isEndOfEntryTable(file)) {
		Entry tempEntry;
		tempEntry.read(file);

		vector<string> ancestorNameList = tempEntry.getAncestorNameList();
		Entry* parent = this->Root->findParent(ancestorNameList);
		if (parent == nullptr) {
			parent = this->Root;
		}
		parent->add(tempEntry);
	}
}

