#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "Console.h"
using namespace std;

class Entry
{
public:
	Entry();
	Entry(Entry const& entry);
	Entry(uint32_t const& size, string const& path, string const& password);
	virtual ~Entry() = default;

	void read(ifstream& file);
	
	bool isFolder() const;
	bool isLocked() const;
	bool hasName(string const& name) const;
	vector<string> getAncestorNameList() const;
	string getPath() const;
	
	virtual Entry* findParent(vector<string>& ancestorNameList) const;
	virtual void add(Entry const& entry);

	void write(ofstream& file) const;

	virtual void display(bool selected);
	virtual void show(int selected) {};
	virtual int getListSize() { return 0; };
	virtual Entry* getEntryInList(int i) { return  nullptr; };
	void setPassword(string pw);
	void resetPassword();
	bool checkPassword(string pw);

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

