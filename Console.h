#pragma once
#include <string>
#include <iostream>
#include <Windows.h>

using namespace std;

void printSpace(int n);
string numCommas(int value);

//Console
void setColor(int textColor, int bgColor);
void clrscr();
void gotoXY(const unsigned int& x, const unsigned int& y);
unsigned int whereX();
unsigned int whereY();

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
