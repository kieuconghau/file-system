#include "EntryTable.h"


EntryTable::EntryTable()
{
	this->Root = new Folder();
}

EntryTable::~EntryTable()
{
	if (this->Root != nullptr) {
		delete this->Root;
		this->Root = nullptr;
	}
}

void EntryTable::read(fstream& file, VolumeInfo const& volumeInfo)
{
	while (!volumeInfo.isEndOfEntryTable_g(file)) {
		Entry entry;
		entry.read(file);
		this->add(entry);
	}
}

void EntryTable::write(fstream& file) const
{
	for (Entry* entry : this->EntryList) {
		entry->write(file);
	}
}

void EntryTable::add(Entry const& entry)
{
	bool foundParent = false;
	
	for (size_t i = 0; i < this->EntryList.size(); ++i) {
		if (entry.hasParent(this->EntryList[i])) {
			this->EntryList.push_back(this->EntryList[i]->add(entry));
			foundParent = true;
			return;
		}
	}

	if (!foundParent) {
		this->EntryList.push_back(this->Root->add(entry));
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

