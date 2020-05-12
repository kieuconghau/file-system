#pragma once

#include "Console.h"

#include <windows.h>
#include <iomanip>
#include <conio.h>

#define W 1017
#define H 572

class GUI
{
public:
	static int line;
	static bool esc;

public:
	static void setWindows(int height, int width);
	static void clearBackground();
	static void reset();
	static string enterPassword();

private:
	static string hidePassword();
};

