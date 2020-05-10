#pragma once
#include <windows.h>
#include <iomanip>
#include <conio.h>
#include "Console.h"
#include "Entry.h"

#define W 1017
#define H 572


class GUI
{
private:
	int line;

public:
	GUI() : line(0) {};
	void setWindows(int height, int width);
	void updateMenu(Entry* f);
	void Function();
	void Navigation(Entry* f);
};

