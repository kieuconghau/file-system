#include "Cache.h"

string const Cache::Path = "cache-file-system";
string const Cache::Signature = "Cache--NHT--FinalProject";

Cache::Cache()
{
	if (this->exist()) {
		this->read();
	}
	else {
		this->initialize();
	}
}

Cache::~Cache() {}

bool Cache::isEmpty() const
{
	return this->VolumePathList.size() == 0;
}

void Cache::add(string const& volumeFilePath)
{
	this->VolumePathList.push_back(volumeFilePath);

	fstream file(this->Path, ios_base::app);
	if (file.is_open()) {
		file.clear();

		file << volumeFilePath << "\n";

		file.close();
	}
	else {
		throw "Cache's File Name Error";
	}
}

void Cache::update()
{
	// Open each Volume File Path in the Volume Path List,
	// find and discard all path which did not exist.
	for (size_t i = 0; i < this->VolumePathList.size(); ) {
		fstream file(this->VolumePathList[i], ios_base::in);

		if (file.is_open()) {
			file.close();
			++i;
		}
		else {
			auto temp = this->VolumePathList[i];
			this->VolumePathList[i] = this->VolumePathList.back();
			this->VolumePathList.back() = temp;

			this->VolumePathList.pop_back();
		}
	}

	// Update file
	fstream file(this->Path, ios_base::out);
	if (file.is_open()) {
		file.clear();

		file << this->Signature << "\n";
		for (string volumeFilePath : this->VolumePathList) {
			file << volumeFilePath << "\n";
		}

		file.close();
	}
}

vector<string> Cache::getVolumePathList() const
{
	return this->VolumePathList;
}

bool Cache::exist()
{
	fstream file(this->Path, ios_base::in);
	if (file.is_open()) {
		file.close();
		return true;
	}

	return false;
}

void Cache::initialize()
{
	fstream file(this->Path, ios_base::out);
	if (file.is_open()) {
		file.clear();

		file << this->Signature << "\n";

		file.close();
	}
}

void Cache::read()
{
	fstream file(this->Path, ios_base::in);
	if (file.is_open()) {
		file.clear();

		string str;

		getline(file, str, '\n');
		if (str != this->Signature) {
			throw "Cache File's Signature Error";
		}

		while (getline(file, str, '\n')) {
			this->VolumePathList.push_back(str);
		}

		file.close();
	}
}

