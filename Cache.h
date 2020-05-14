#pragma once

#include "GUI.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class Cache
{
public:
	Cache();
	~Cache();

	bool isEmpty() const;
	void add(string const& volumeFilePath);
	void update();
	void clear();
	void showListOfRecentlyOpenedVolume() const;
	bool hasVolume(string& str) const;

private:
	static string const Path;
	static string const Signature;
	vector<string> VolumePathList;
private:
	bool exist();
	void initialize();
	void read();
};

