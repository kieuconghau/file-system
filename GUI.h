#pragma once

#include "Console.h"
#include <windows.h>
#include <iomanip>
#include <conio.h>

#define W 1050
#define H 572

#define w 123
#define h 33

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

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
	static void printProgress(string content, double percentage);
	static void instructionScreen();

private:
	static string hidePassword();
};

