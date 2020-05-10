#include "File.h"

File::File(Entry const& entry) : Entry(entry) {}

Entry* File::findParent(vector<string>& ancestorNameList) const
{
	return nullptr;
}

void File::add(Entry const& entry) {}

void File::display(bool selected) {
    if (selected) setColor(15, 1);

    int y = whereY();
    gotoXY(73, y);
    cout << "File      ";
    gotoXY(0, y);

    if (selected) setColor(15, 0);
    Entry::display(selected);
}