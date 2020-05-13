#pragma once

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
	vector<string> getVolumePathList() const;

private:
	static string const Path;
	static string const Signature;
	vector<string> VolumePathList;
private:
	bool exist();
	void initialize();
	void read();
};

