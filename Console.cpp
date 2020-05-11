#include "Console.h"

void setColor(int textColor, int bgColor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (textColor + (bgColor * 16)));
}

void clrscr()
{
    CONSOLE_SCREEN_BUFFER_INFO	csbiInfo;
    HANDLE	hConsoleOut;
    COORD	Home = { 0,0 };
    DWORD	dummy;

    hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);

    FillConsoleOutputCharacter(hConsoleOut, ' ', csbiInfo.dwSize.X * csbiInfo.dwSize.Y, Home, &dummy);
    csbiInfo.dwCursorPosition.X = 0;
    csbiInfo.dwCursorPosition.Y = 0;
    SetConsoleCursorPosition(hConsoleOut, csbiInfo.dwCursorPosition);
}

void gotoXY(const unsigned int& x, const unsigned int& y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void printSpace(int n) {
    for (int i = 0; i < n / 10; i++) {
        cout << "          ";
    }

    int r = n / 10;
    n = n - r * 10;

    for (int i = 0; i < n; i++) {
        cout << " ";
    }
}

string numCommas(int value) {
    string numWithCommas = to_string(value);
    int insertPosition = numWithCommas.length() - 3;
    while (insertPosition > 0) {
        numWithCommas.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    return numWithCommas;
}

unsigned int whereX() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition.X;
}

unsigned int whereY() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.dwCursorPosition.Y;
}

void sleep(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(x));
}

unsigned int SHF(string input) {
    unsigned int Init = 124564352;
    unsigned int hashcode = 5674356;
    unsigned int hash;

    for (int i = 0; i < input.length(); i++) {
        hash = hash ^ (input[i]);
        hash = hash * hashcode;
    }

    return hash;
}

string toHex(unsigned int input) {
    string hexhash;
    stringstream hexstream;
    hexstream << hex << input;
    hexhash = hexstream.str();
    std::transform(hexhash.begin(), hexhash.end(), hexhash.begin(), ::toupper);
    return hexhash;
}