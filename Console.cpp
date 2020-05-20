#include "Console.h"

void setColor(int textColor, int bgColor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (textColor + (bgColor * 16)));
}

void printTextAtMid(string const& text, size_t const& left, size_t const& right)
{
    gotoXY((right - left + text.length()) / 2, whereY());
    cout << text;
}


void setColor(COLOR textColor, COLOR bgColor)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), ((size_t)textColor + ((size_t)bgColor * 16)));
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

string numCommas(uint64_t value) {
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

string addSalt(string pw) {
    uint8_t salt[3] = { 0x4E , 0x48 ,0x54 };

    // Random positon of salt
    int r1 = rand() % (pw.length());
    int r2 = rand() % (pw.length());
    int r3 = rand() % (pw.length());

    // XOR with salt
    pw[r1] = pw[r1] ^ salt[0];
    pw[r2] = pw[r2] ^ salt[1];
    pw[r3] = pw[r3] ^ salt[2];

    return pw;
}

string addPepper(string pw) {
    char pepper = rand() % (255 + 1);
    
    pw = pw + pepper;

    return pw;
}

void FixConsoleWindow() {
    HWND consoleWindow = GetConsoleWindow();
    LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
    style = style & ~(WS_MAXIMIZEBOX) & ~(WS_THICKFRAME);
    SetWindowLong(consoleWindow, GWL_STYLE, style);
}

void FixSizeWindow(int width, int height) {
    _COORD coord;
    coord.X = width;
    coord.Y = height;

    _SMALL_RECT Rect;
    Rect.Top = 0;
    Rect.Left = 0;
    Rect.Bottom = height  - 169; // 200 - 169 la hien thi duoc 32 dong, scroll xuong thi duoc 200 dong
    Rect.Right = width - 1;

    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);      // Get Handle
    SetConsoleScreenBufferSize(Handle, coord);            // Set Buffer Size
    SetConsoleWindowInfo(Handle, TRUE, &Rect);            // Set Window Size
}

string convert(uint8_t *str) {
    return string((char*)str);
}