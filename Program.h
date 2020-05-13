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
	void openVolumeInCache();
	void createVolume();
	
	void initializeVolume();
	void closeVolume();
	
	void homeScreen();
	void homeNavigate();
};
