#pragma once

#include <iostream>
#include <vector>
using namespace std;

class Cache
{
public:
	Cache();
	~Cache();



private:
	static string const Path;
	vector<string> volumePathList;
};

