#include "Folder.h"

Folder::Folder(Entry const& entry) : Entry(entry) {}

Folder::~Folder()
{
	for (size_t i = 0; i < this->EntryList.size(); ++i) {
		if (this->EntryList[i] != nullptr) {
			delete this->EntryList[i];
			this->EntryList[i] = nullptr;
		}
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

void Folder::display(bool selected) {
	if (selected) setColor(15, 1);

	int y = whereY();
	gotoXY(73, y);
	cout << "Folder    ";
	gotoXY(0, y);

	if (selected) setColor(15, 0);
	Entry::display(selected);
}

void Folder::displayParent(bool selected) {
	if (selected) setColor(15, 1);

	cout << " .."; printSpace(120);

	int y = whereY();
	gotoXY(73, y);
	cout << "Folder    ";
	gotoXY(0, y + 1);

	if (selected) setColor(15, 0);
}

void Folder::show(int selected) {
	displayParent(selected == 0);

	for (int i = 0; i < EntryList.size(); i++) {
		EntryList[i]->display(selected - 1 == i);
	}
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

bool Folder::hasChildWithTheSameName(Entry const& entry) const
{
	for (size_t i = 0; i < this->EntryList.size(); ++i) {
		if (this->EntryList[i]->getName() == entry.getName()) {
			return true;
		}
	}

	return false;
}

