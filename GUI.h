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
	bool esc;

public:
	GUI() : line(0), esc(false) {};
	void setWindows(int height, int width);
	void updateMenu(Entry* f);
	void Navigation(Entry* f);
	void reset();
	void clearBackground();
	string HidePassword();
	string EnterPassword();
	void SetPassword(Entry* f);
	void EnterFolder(Entry* parent, bool &back);

};

