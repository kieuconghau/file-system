#pragma once

#include "Entry.h"

class File : public Entry
{
public:
	File() = default;
	File(Entry const& entry);
	~File() = default;

	Entry* findParent(vector<string>& ancestorNameList) const override;
	Entry* add(Entry const& entry) override;
	void del() override;
};

