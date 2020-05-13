#pragma once

#include "Volume.h"
#include "Cache.h"

class Program
{
public:
	Program();
	~Program();

	void run();

private:
	Volume* Vol;
	Cache Cache;
private:
	void openVolume();
	void createVolume();
	
	void initializeVolume(string const& volumeFilePath);
	void closeVolume();
	
	void homeScreen();
	void homeNavigate();
};
