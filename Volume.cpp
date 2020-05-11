#include "Volume.h"

Volume::Volume() : VolumeInfo(), EntryTable()
{
	this->Path = "";
}

void Volume::create(string const& volumeFilePath)
{
	this->Path = volumeFilePath;

	fstream file(this->Path, ios_base::out);
	if (file.is_open()) {
		this->VolumeInfo.write(file);
	}
	file.close();
}

void Volume::open(string const& volumeFilePath)
{
	this->Path = volumeFilePath;
	
	fstream file(this->Path);
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
	- Set/Remove Password:	P
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

	fstream file(this->Path, ios_base::in);
	if (file.is_open()) {
		this->seekToHeadOfVolumeInfo(file);
		this->VolumeInfo.read(file);
		isVF = this->VolumeInfo.checkSignature(file);
	}
	file.close();

	return isVF;
}

void Volume::del(Entry* entry, Entry const* parent)
{
	fstream file(this->Path);
	if (file.is_open()) {
		file.clear();

		// Data Field
		size_t const BLOCK_SIZE = 4096;	// byte
		uint8_t subData[BLOCK_SIZE];

		entry->seekToHeadOfData(file);
		size_t startWrite = (size_t)file.tellg();

		entry->seekToEndOfData(file);
		size_t startRead = (size_t)file.tellg();

		this->VolumeInfo.seekToHeadOfEntryTable(file);
		size_t endDataField = (size_t)file.tellg();

		size_t shiftingDataSize = endDataField - startRead;

		for (size_t i = 0; i < shiftingDataSize / BLOCK_SIZE; ++i) {
			file.seekg(startRead);
			file.read((char*)subData, BLOCK_SIZE);
			startRead += BLOCK_SIZE;

			file.seekg(startWrite);
			file.write((char*)subData, BLOCK_SIZE);
			startWrite += BLOCK_SIZE;
		}
		shiftingDataSize %= BLOCK_SIZE;		// remain
		file.seekg(startRead);
		file.read((char*)subData, shiftingDataSize);
		file.seekg(startWrite);
		file.write((char*)subData, shiftingDataSize);

		// Entry Table
		this->EntryTable.updateAfterDel(entry);
		this->EntryTable.write(file);

		// Volume Info
		this->VolumeInfo.updateAfterDel(entry);
		this->VolumeInfo.write(file);

		// Delete entry
		entry->del();
	}
	file.close();
}

void Volume::seekToHeadOfVolumeInfo(fstream& file) const
{
	file.seekg(0 - (int)sizeof(VolumeInfo), ios_base::end);
}

void Volume::seekToHeadOfEntryTable(fstream& file) const
{
	this->VolumeInfo.seekToHeadOfEntryTable(file);
}

