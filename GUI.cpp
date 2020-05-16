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
    gotoXY(0, 31);
    printSpace(123);
    gotoXY(0, 0);

    setColor(15, 0);
}

void GUI::reset() {
    line = 0;
}

string GUI::hidePassword() {
    setColor(COLOR::WHITE, COLOR::BLACK);
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

    setColor(COLOR::LIGHT_CYAN, COLOR::BLACK);

    gotoXY(0, 0);
    GUI::printTextAtMid("===== LOGIN =====");

    while (true) {
        setColor(COLOR::LIGHT_CYAN, COLOR::BLACK);

        gotoXY(0, 2); 
        cout << "  Password:  ";      pw   = GUI::hidePassword();      cout << endl;
        setColor(COLOR::LIGHT_CYAN, COLOR::BLACK);
        cout << "  Re-enter:  ";      repw = GUI::hidePassword();      cout << endl;

        if (pw.compare(repw) == 0) {
            break;
        }
        else {
            clrscr();
            setColor(COLOR::LIGHT_RED, COLOR::BLACK);
            gotoXY(0, 0);
            cout << "  Error: Password is not the same. " << endl;
            setColor(COLOR::WHITE, COLOR::BLACK);
        }
    }
    return pw;
}

void GUI::checkPassword(int i) {
    i = i % 3;

    setColor(COLOR::LIGHT_CYAN, COLOR::BLACK);
    gotoXY(0, 5);
    switch (i) {
    case 0:
        cout << "  Verifing password  .      ";
        break;
    case 1:
        cout << "  Verifing password  .  .   ";
        break;
    case 2:
        cout << "  Verifing password  .  .  .";
        break;
    }
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

    setColor(COLOR::LIGHT_CYAN, COLOR::BLACK);
    gotoXY(0, 0);
    GUI::printTextAtMid("===== .NHT INSTRUCTION =====");

    setColor(COLOR::WHITE, COLOR::BLACK);
    gotoXY(0, 2);  cout << "  * Use ARROW 'UP' and ARROW 'DOWN' to move upward and downward between lines. ";

    gotoXY(0, 4);  cout << "  * When you open an existing volume successfully these are functional key you should know:";

    size_t x1 = 8;
    size_t x2 = 24;

    setColor(COLOR::LIGHT_CYAN, COLOR::BLACK);
    gotoXY(x1, 6); cout << "[ENTER]:";
    setColor(COLOR::WHITE, COLOR::BLACK);
    gotoXY(x2, 6); cout << "To enter a folder.";

    setColor(COLOR::LIGHT_CYAN, COLOR::BLACK);
    gotoXY(x1, 8); cout << "[P]:";
    setColor(COLOR::WHITE, COLOR::BLACK);
    gotoXY(x2, 8); cout << "To set / reset password for a folder or file.";
    gotoXY(x2, 9); cout << "If a folder/file has password, it requires password to do any manipulation on it.";

    setColor(COLOR::LIGHT_CYAN, COLOR::BLACK);
    gotoXY(x1, 11); cout << "[I]:";
    setColor(COLOR::WHITE, COLOR::BLACK);
    gotoXY(x2, 11); cout << "To import a file/folder from a given path.";

    setColor(COLOR::LIGHT_CYAN, COLOR::BLACK);
    gotoXY(x1, 13); cout << "[E]:";
    setColor(COLOR::WHITE, COLOR::BLACK);
    gotoXY(x2, 13); cout << "To export a file/folder from a given path.";

    setColor(COLOR::LIGHT_CYAN, COLOR::BLACK);
    gotoXY(x1, 15); cout << "[DEL] or [D]:";
    setColor(COLOR::WHITE, COLOR::BLACK);
    gotoXY(x2, 15); cout << "To delete a file or a folder";
    gotoXY(x2, 16); cout << "If you delete a folder which has a locked file/folder inside,";
    gotoXY(x2, 17); cout << "It won't be deleted totally and still contain the locked files/folders inside.";

    setColor(COLOR::LIGHT_CYAN, COLOR::BLACK);
    gotoXY(x1, 19); cout << "[F1]:";
    setColor(COLOR::WHITE, COLOR::BLACK);
    gotoXY(x2, 19); cout << "Show this instruction.";

    gotoXY(0, 21); cout << "  ";  system("pause");
}

void GUI::printTextAtMid(string const& text, size_t const& left, size_t const& right)
{
    gotoXY((right - left - text.length()) / 2, whereY());
    cout << text;
}

void GUI::displayParent(bool selected) {
    if (selected) setColor(15, 1);

    cout << " .."; printSpace(120);

    int y = whereY();
    gotoXY(73, y);
    cout << "Folder    ";
    gotoXY(0, y + 1);

    if (selected) setColor(15, 0);
}