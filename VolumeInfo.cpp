#include "VolumeInfo.h"

VolumeInfo::VolumeInfo()
{
	this->Signature			= 0x024E4854;	// "THN "
	this->SizeEntryTable	= 0;
	this->OffsetEntryTable	= 0;
}

VolumeInfo::~VolumeInfo() {}

void VolumeInfo::write(fstream& file) const
{
	file.write((char*)&this->Signature, sizeof(this->Signature));
	file.write((char*)&this->SizeEntryTable, sizeof(this->SizeEntryTable));
	file.write((char*)&this->OffsetEntryTable, sizeof(this->OffsetEntryTable));
}

void VolumeInfo::read(fstream& file) const
{
	file.read((char*)&this->Signature, sizeof(this->Signature));
	file.read((char*)&this->SizeEntryTable, sizeof(this->SizeEntryTable));
	file.read((char*)&this->OffsetEntryTable, sizeof(this->OffsetEntryTable));
}

bool VolumeInfo::checkSignature(fstream& file) const
{
	return this->Signature == 0x024E4854;
}

void VolumeInfo::seekToHeadOfEntryTable(fstream& file) const
{
	file.clear();
	file.seekg(this->OffsetEntryTable);
}

bool VolumeInfo::isEndOfEntryTable(fstream& file) const
{
	return file.tellg() == this->OffsetEntryTable + this->SizeEntryTable;
}

void VolumeInfo::updateAfterDel(Entry const* entry)
{
	this->SizeEntryTable -= entry->getSize();
	this->OffsetEntryTable -= entry->getSizeData();
}

void VolumeInfo::updateAfterSetPassword(size_t const& oldPasswordLen, size_t const& newPasswordLen)
{
	this->SizeEntryTable += (newPasswordLen - oldPasswordLen);
}
