#include "Folder.h"

Folder::Folder(Entry const& entry) : Entry(entry) {}

Folder::~Folder()
{
	for (Entry* entry : this->EntryList) {
		delete entry;
		entry = nullptr;
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

void Folder::del()
{

}
