#include "Folder.h"

Folder::Folder(Entry const& entry) : Entry(entry) {}

Folder::~Folder()
{
	for (Entry* entry : this->EntryList) {
		delete entry;
		entry = nullptr;
	}

	this->EntryList.resize(0);
	this->EntryList.shrink_to_fit();
}

Entry* Folder::add(Entry const& tempEntry) {
	Entry* entry;

	if (tempEntry.isFolder()) {
		entry = new Folder(tempEntry);
	}
	else {
		entry = new File(tempEntry);
	}

	this->EntryList.push_back(entry);

	return entry;
}

void Folder::del(Entry* entry)
{
	for (auto entryIterator = this->EntryList.begin(); entryIterator != this->EntryList.end(); ++entryIterator) {
		if (*entryIterator == entry) {
			delete *entryIterator;
			this->EntryList.erase(entryIterator);
			this->EntryList.shrink_to_fit();
			return;
		}
	}

	throw "Logic Error";
}

vector<Entry*> Folder::getSubEntryList() const
{
	return this->EntryList;
}
