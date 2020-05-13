#pragma once

#include "Volume.h"

class Program
{
public:
	Program();
	~Program();

	void run();

private:
	Volume* Vol;
private:
	void openVolume();
	void createVolume();
	void closeVolume();
	void homeScreen();
	void homeNavigate();
	void instructionScreen();
};

