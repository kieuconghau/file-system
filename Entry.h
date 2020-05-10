#pragma once
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class Entry
{
public:
	Entry();
	Entry(Entry const& entry);
	virtual ~Entry() = default;

	void read(ifstream& file);
	
	bool isFolder() const;
	bool hasName(string const& name) const;
	vector<string> getAncestorNameList() const;
	
	virtual Entry* findParent(vector<string>& ancestorNameList) const;
	virtual void add(Entry const& entry);

private:
	void splitPath();

protected:
	/*==========*/
	uint16_t	ModifiedTime;
	uint16_t	ModifiedDate;
	uint32_t	Size;
	uint16_t	PathLen;
	uint16_t	PasswordLen;
	uint32_t	OffsetData;
	string		Path;
	string		Password;
	/*==========*/
	string Name;

private:
	vector<string>		AncestorNameList;
	static const char	SLASH = '/';
};

