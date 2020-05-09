#include "Volume.h"

Volume::Volume() : VolumeInfo(), EntryTable()
{
	this->Path = "";
}

void Volume::create(string const& volumeFilePath)
{
	this->Path = volumeFilePath;

	ofstream file(this->Path, ios_base::out);
	if (file.is_open()) {
		this->VolumeInfo.write(file);
	}
	file.close();
}

void Volume::open(string const& volumeFilePath)
{
	this->Path = volumeFilePath;
	
	ifstream file(this->Path, ios_base::in);
	if (file.is_open()) {
		this->seekToHeadOfVolumeInfo(file);
		this->VolumeInfo.read(file);

		this->seekToHeadOfEntryTable(file);
		this->EntryTable.read(file, this->VolumeInfo);
	}
	file.close();


	/*
	Show list of files/folders in this volume.
	Press hotkeys to implement some funtions:
	- Import:				I
	- Export:				E
	- Delete:				D
	- Set/Cancel Password:	P
	- Move down:			Down
	- Move up:				Up
	- Open folder:			Enter
	- Back:					Backspace
	- Exit:					Escape
	*/
}

bool Volume::isVolumeFile(string const& volumeFilePath)
{
	bool isVF = false;
	this->Path = volumeFilePath;

	ifstream file(this->Path, ios_base::in);
	if (file.is_open()) {
		this->seekToHeadOfVolumeInfo(file);
		this->VolumeInfo.read(file);
		isVF = this->VolumeInfo.checkSignature(file);
	}
	file.close();

	return isVF;
}

void Volume::seekToHeadOfVolumeInfo(ifstream& file) const
{
	file.clear();
	file.seekg(0 - (int)sizeof(VolumeInfo), ios_base::end);
}

void Volume::seekToHeadOfEntryTable(ifstream& file) const
{
	this->VolumeInfo.seekToHeadOfEntryTable(file);
}

