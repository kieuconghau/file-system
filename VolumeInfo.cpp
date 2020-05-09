#include "VolumeInfo.h"

VolumeInfo::VolumeInfo()
{
	this->Signature			= 0x024E4854;
	this->SizeEntryTable	= 0;
	this->OffsetEntryTable	= 0;
}

void VolumeInfo::write(ofstream& file) const
{
	file.write((char*)&this->Signature, sizeof(this->Signature));
	file.write((char*)&this->SizeEntryTable, sizeof(this->SizeEntryTable));
	file.write((char*)&this->OffsetEntryTable, sizeof(this->OffsetEntryTable));
}

void VolumeInfo::read(ifstream& file) const
{
	file.read((char*)&this->Signature, sizeof(this->Signature));
	file.read((char*)&this->SizeEntryTable, sizeof(this->SizeEntryTable));
	file.read((char*)&this->OffsetEntryTable, sizeof(this->OffsetEntryTable));
}

bool VolumeInfo::checkSignature(ifstream& file) const
{
	return this->Signature == 0x024E4854;
}

void VolumeInfo::seekToHeadOfEntryTable(ifstream& file) const
{
	file.clear();
	file.seekg(this->OffsetEntryTable);
}

