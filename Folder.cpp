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
	Parent->displayParent(selected == 0);

	for (int i = 0; i < EntryList.size(); i++) {
		EntryList[i]->display(selected - 1 == i);
	}
}