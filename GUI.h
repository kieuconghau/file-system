#pragma once

#include "Console.h"
#include <windows.h>
#include <iomanip>
#include <conio.h>

#define w 123
#define h 32

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
	static void checkPassword(int i);
	static void printProgress(string content, double percentage);
	static void instructionScreen();
	static void printTextAtMid(string const& text, size_t const& left = 0, size_t const& right = w);

private:
	static string hidePassword();
};

