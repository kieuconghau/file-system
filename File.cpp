#include "File.h"

File::File(Entry const& entry) : Entry(entry) {}

File::~File() {}

Entry* File::add(Entry const& entry) { return nullptr; }

void File::del(Entry* entry) {}

vector<Entry*> File::getSubEntryList() const
{
	return vector<Entry*>();
}
