#pragma once

#include "GUI.h"
#include "VolumeInfo.h"
#include "EntryTable.h"

#include <Windows.h>
#include <queue>

enum class ExportState {
	SUCCESS = 0,
	NOT_TOTALLY,
	SAME_NAME,
	BAD_PATH
};

class Volume
{
public:
	static string const Extension;

public:
	Volume(string const& volumeFilePath);
	~Volume();

	bool isVolumeFile();
	bool create();
	void open();
	bool isEmpty() const;

	string getPath() const;

private:
	void initialize(string const& volumeFilePath);

	void seekToHeadOfVolumeInfo_g(fstream& file) const;
	void seekToHeadOfVolumeInfo_p(fstream& file) const;
	void seekToHeadOfEntryTable_g(fstream& file) const;
	void seekToHeadOfEntryTable_p(fstream& file) const;

	void performFunctions();
	void navigate(Entry* entry);
	void updateMenu(Entry* entry);
	bool enterFolder(Entry* parent, bool& back);

	void setPassword(Entry* f);
	void writePasswordChange();

	void deleteOnVolume(Entry* f);
	bool del(Entry* entry, Entry* parent);

	void importGUI(Entry* parent);
	bool import(string const& new_file_path, Entry* parent);
	
	void exportGUI(Entry* f);
	ExportState exportFile(Entry* export_file_entry, string const& destination_path);

	void resize(uint64_t const& size);

private:
	string		Path;
	VolumeInfo	VolumeInfo;
	EntryTable	EntryTable;
};

