#include "File.h"

File::File(Entry const& entry) : Entry(entry) {}

File::~File() {}

Entry* File::add(Entry const& entry) { return nullptr; }

void File::del(Entry* entry) {}

vector<Entry*> File::getSubEntryList() const
{
	return vector<Entry*>();
}

bool File::hasChildWithTheSameName(Entry const& entry) const
{
	return true;
}

void File::display(bool selected) {
    if (selected) setColor(15, 1);

    int y = whereY();
    gotoXY(73, y);
    cout << "File      ";
    gotoXY(0, y);

    if (selected) setColor(15, 0);
    Entry::display(selected);
}

