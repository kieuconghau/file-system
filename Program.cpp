#include "Program.h"

Program::Program()
{
	GUI::setWindows(W, H);
	this->Vol = nullptr;
}

Program::~Program()
{
	delete this->Vol;
}

void Program::run()
{
	this->openVolume();
}

void Program::openVolume()
{
	this->Vol = new Volume();

	string volumeFilePath;
	cout << "Input path: ";
	getline(cin, volumeFilePath);

	if (this->Vol->isVolumeFile(volumeFilePath)) {
		this->Vol->open(volumeFilePath);
	}
	else {
		this->closeVolume();
	}
}

void Program::createVolume()
{
	this->Vol = new Volume();

	string volumeFilePath;
	cout << "Input path: ";
	getline(cin, volumeFilePath);

	this->Vol->create(volumeFilePath);

	this->closeVolume();
}

void Program::closeVolume()
{
	delete this->Vol;
	this->Vol = nullptr;
}

