#include "EntryTable.h"

EntryTable::EntryTable()
{
	this->Root = new Folder();
}

EntryTable::~EntryTable()
{
	delete this->Root;
	this->Root = nullptr;
}

void EntryTable::read(fstream& file, VolumeInfo const& volumeInfo)
{
	while (!volumeInfo.isEndOfEntryTable(file)) {
		Entry tempEntry;
		tempEntry.read(file);

		vector<string> ancestorNameList = tempEntry.AncestorNameList;
		Entry* parent = this->Root->findParent(ancestorNameList);
		if (parent == nullptr) {
			parent = this->Root;
		}

		this->EntryList.push_back(parent->add(tempEntry));
	}
}

void EntryTable::updateAfterDel(Entry const* entry)
{
	auto delEntry = this->EntryList.begin();
	auto entryIterator = this->EntryList.begin();

	// Find the Entry that we want to delete
	for ( ; entryIterator != this->EntryList.end(); ++entryIterator) {
		if (*entryIterator == entry) {
			delEntry = entryIterator;
			break;
		}
	}

	if (entryIterator == this->EntryList.end()) {
		throw "Logic Error";
	}

	// Update OffsetData of all entries behind the entry that we want to delete
	for (++entryIterator; entryIterator != this->EntryList.end(); ++entryIterator) {
		(*entryIterator)->OffsetData -= (*delEntry)->SizeData;
	}

	// Delete entry from EntryList
	this->EntryList.erase(delEntry);
	this->EntryList.shrink_to_fit();
}

void EntryTable::write(fstream& file) const
{
	for (auto entry = this->EntryList.begin(); entry != this->EntryList.end(); ++entry) {
		(*entry)->write(file);
	}
}
