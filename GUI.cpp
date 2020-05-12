#include "GUI.h"

int GUI::line = 0;
bool GUI::esc = false;

void GUI::setWindows(int height, int width) {
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r); //stores the console's current dimensions

    MoveWindow(console, r.left, r.top, height, width, TRUE); // 1024 width, 576 height
}

void GUI::clearBackground() {
    setColor(0, 0);

    printSpace(123); cout << endl;
    printSpace(123);
    gotoXY(0, line + 2);
    printSpace(123); 
    gotoXY(0, line + 25);
    printSpace(123);
    gotoXY(0, 0);

    setColor(15, 0);
}

void GUI::reset() {
    line = 0;
}

string GUI::hidePassword() {
    string input;
    char temp;
    while (true) {
        temp = _getch();
        if (temp == '\r') {
            break;
        }
        else if (input.length() > 0 && temp == 8) {
            input.pop_back();
            std::cout << "\b \b";
        }
        else if (input.length() == 0 && temp == 8) {
            continue;
        }
        else {
            std::cout << temp;
            Sleep(50);
            std::cout << "\b*";
            input.push_back(temp);
        }
    }
    return input;
}

string GUI::enterPassword() {
    string pw, repw;

    while (true) {

        cout << "Password:  ";      pw   = GUI::hidePassword();      cout << endl;
        cout << "Re-enter:  ";      repw = GUI::hidePassword();      cout << endl;

        if (pw.compare(repw) == 0) {
            break;
        }
        else {
            clrscr();
            setColor(4, 0);
            cout << "Error: Password is not the same. " << endl;
            setColor(15, 0);
        }
    }
    return pw;
}

void GUI::printProgress(string content, double percentage) {
    int val = (int)(percentage * 100);
    int lpad = (int)(percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;

    setColor(13, 0);
    cout << content; gotoXY(10 - content.length(), whereY());

    setColor(14, 0);
    printf("%3d%%", val);

    setColor(10, 0);
    printf("  [%.*s%*s]\n", lpad, PBSTR, rpad, ""); 

    setColor(15, 0);
    fflush(stdout);
}