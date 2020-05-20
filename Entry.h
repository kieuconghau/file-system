#pragma once

#include "Console.h"
#include "SHA256.h"
#include "GUI.h"
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

class Entry
{
	friend class EntryTable;

public:
	static const char SLASH = '/';

public:
	Entry();
	Entry(Entry const& entry);
	virtual ~Entry();

	void read(fstream& file);
	void write(fstream& file) const;
	
	bool isFolder() const;
	bool isLocked() const;
	bool hasName(string const& name) const;
	bool hasParent(Entry const* parent) const;
	
	string getPath() const;
	uint32_t getSizeData() const;
	uint32_t getSize() const;
	uint16_t getPasswordLen() const;
	string getFullPathOutside() const;
	bool getIsFolder() const;

	virtual Entry* add(Entry const& entry);
	virtual void del(Entry* entry);
	virtual vector<Entry*> getSubEntryList() const;
	virtual bool hasChildWithTheSameName(Entry const& entry) const;

	void write(ofstream& file) const;

	/* ============== BUU WRITE THIS ============== */
	virtual void display(bool selected);
	virtual void show(int selected) {};
	virtual int getListSize() { return 0; };
	virtual Entry* getEntryInList(int i) { return  nullptr; };
	void setPassword(string pw);
	void resetPassword();
	bool checkPassword(string pw);
	string getName() const { return this->Name; };
	void displayModTime();
	void displayModDate();
	/* ============== BUU WRITE THIS ============== */
  
	void seekToHeadOfData_g(fstream& file) const;
	void seekToHeadOfData_p(fstream& file) const;
	void seekToEndOfData_g(fstream& file) const;
	void seekToEndOfData_p(fstream& file) const;

	void getFileInfoAndConvertToEntry(_WIN32_FIND_DATAA ffd, string file_path, string file_name_in_volume, uint64_t& insert_pos);
	void standardizeAfterImport(Entry* parent);

private:
	void initializeName();
	void standardizePath();
	void updatePathAfterImport(Entry* parent);

protected:
	/*==========*/
	uint16_t	ModifiedTime;
	uint16_t	ModifiedDate;
	uint32_t	SizeData;
	uint16_t	PathLen;
	uint16_t	PasswordLen;
	uint64_t	OffsetData;
	string		Path;
	string		Password;
	/*==========*/
	string		Name;
	string		FullPathOutside;	// Window
	bool		IsFolder;
};

