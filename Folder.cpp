#include "Folder.h"

Folder::Folder()
{
	this->Parent = nullptr;
}

Folder::Folder(Entry const& entry, Folder* parent) : Entry(entry)
{
	this->Parent = parent;
}

Folder::~Folder()
{
	this->Parent = nullptr;

	for (Entry* entry : this->EntryList) {
		delete entry;
	}
}

Entry* Folder::findParent(vector<string>& ancestorNameList) const
{
	if (ancestorNameList.size() == 0)
		return nullptr;

	for (Entry* entry : this->EntryList) {
		if (entry->isFolder() && entry->hasName(*ancestorNameList.begin())) {
			if (ancestorNameList.size() == 1) {
				return entry;
			}
			ancestorNameList.erase(ancestorNameList.begin());
			ancestorNameList.shrink_to_fit();
			return entry->findParent(ancestorNameList);
		}
	}

	throw "Logic Error";
	return nullptr;
}

void Folder::add(Entry const& tempEntry) {
	Entry* entry;

	if (tempEntry.isFolder()) {
		entry = new Folder(tempEntry, this);
	}
	else {
		entry = new File(tempEntry);
	}

	this->EntryList.push_back(entry);
}