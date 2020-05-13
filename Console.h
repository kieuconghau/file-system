#pragma once
#include <string>
#include <iostream>
#include <Windows.h>
#include <chrono>
#include <thread>
#include <sstream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>


enum class COLOR {
	BLACK = 0,
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	LIGHT_GRAY,
	DARK_GRAY,
	LIGHT_BLUE,
	LIGHT_GREEN,
	LIGHT_CYAN,
	LIGHT_RED,
	LIGHT_MAGENTA,
	YELLOW,
	WHITE
};


void printSpace(int n);
string numCommas(int value);

// Console
void setColor(int textColor, int bgColor);
void clrscr();
void gotoXY(const unsigned int& x, const unsigned int& y);
unsigned int whereX();
unsigned int whereY();
void sleep(int x);

// Add pepper
string addSalt(string pw);
string addPepper(string pw);

void setColor(COLOR textColor, COLOR bgColor);
void showConsoleCursor(bool showFlag);

/*
Name         | Value
			 |
Black        |   0
Blue         |   1
Green        |   2
Cyan         |   3
Red          |   4
Magenta      |   5
Brown        |   6
Light Gray   |   7
Dark Gray    |   8
Light Blue   |   9
Light Green  |   10
Light Cyan   |   11
Light Red    |   12
Light Magenta|   13
Yellow       |   14
White        |   15
*/
