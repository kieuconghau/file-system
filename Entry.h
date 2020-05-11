#pragma once
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class Entry
{
	friend class EntryTable;
public:
	Entry();
	Entry(Entry const& entry);
	virtual ~Entry() = default;

	void read(fstream& file);
	void write(fstream& file) const;
	
	bool isFolder() const;
	bool isLocked() const;
	bool hasName(string const& name) const;
	
	string getPath() const;
	uint32_t getSizeData() const;
	uint32_t getSize() const;

	virtual Entry* findParent(vector<string>& ancestorNameList) const;
	virtual Entry* add(Entry const& entry);
	virtual void del();

	void seekToHeadOfData(fstream& file) const;
	void seekToEndOfData(fstream& file) const;

private:
	void splitPath();

protected:
	/*==========*/
	uint16_t	ModifiedTime;
	uint16_t	ModifiedDate;
	uint32_t	SizeData;
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

