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

void GUI::instructionScreen() {

    gotoXY(0, 0);  cout << ".NHT INSTRUCTION";

    gotoXY(0, 2);  cout << "* Use ARROW 'UP' and ARROW 'DOWN' to move upward and downward between lines. ";

    gotoXY(0, 4);  cout << "* When you open an existing volume successfully these are functional key you should know:";

    gotoXY(0, 6);  cout << "\t[ENTER]:        To enter a folder.";

    gotoXY(0, 8);  cout << "\t[P]:            To set/reset password for a folder or file.";
    gotoXY(0, 9);  cout << "\t                If a folder/file has password, it requires password to do any manipulation on it.";

    gotoXY(0, 11); cout << "\t[I}:            To import a file/folder from a given path.";

    gotoXY(0, 13); cout << "\t[E]:            To export a file/folder from a given path.";

    gotoXY(0, 15); cout << "\t[DEL] or [D]:   To delete a file or a folder";
    gotoXY(0, 16); cout << "\t                If you delete a folder which has a locked file/folder inside,";
    gotoXY(0, 17); cout << "\t                It won't be deleted totally and still contain the locked files/folders inside.";

    gotoXY(0, 19); cout << "\t[F1]:           Show this instruction.";

    gotoXY(0, 21);  system("pause");
}