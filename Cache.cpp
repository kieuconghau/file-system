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
	for (string str : this->VolumePathList) {
		if (str == volumeFilePath) {
			return;
		}
	}

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

void Cache::clear()
{
	this->VolumePathList.clear();
	this->initialize();
}

void Cache::showListOfRecentlyOpenedVolume() const
{
	if (!this->isEmpty()) {
		cout << "  List of recent volumes:" << "\n";
		for (size_t i = 0; i < this->VolumePathList.size(); ++i) {
			cout << "    <" << i + 1 << "> ";
			gotoXY(10, whereY());
			cout << this->VolumePathList[i] << "\n";
		}
	}
}

bool Cache::hasVolume(string& str) const
{
	if (str.length() != 0 && str.front() == '<' && str.back() == '>') {
		size_t indexVolume = 0;

		for (size_t i = 1; i < str.length() - 1; ++i) {
			if (str[i] >= '0' && str[i] <= '9') {
				indexVolume *= 10;
				indexVolume += str[i] - '0';
			}
			else {
				indexVolume = 0;
				break;
			}
		}

		if (indexVolume >= 1 && indexVolume <= this->VolumePathList.size()) {
			str = this->VolumePathList[indexVolume - 1];
			return true;
		}
	}

	return false;
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

