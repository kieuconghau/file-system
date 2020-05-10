#include "GUI.h"

void GUI::setWindows(int height, int width) {
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r); //stores the console's current dimensions

    MoveWindow(console, r.left, r.top, height, width, TRUE); // 1024 width, 576 height
}

void GUI::updateMenu(Entry* f) {
    clrscr();
    setColor(0, 15);
    cout << " Name"; printSpace(42); cout << " | "; printSpace(16); cout << "Size | Type"; printSpace(6); cout << " | Modified"; printSpace(16); cout << " |  Password " << endl;
    setColor(15, 0);

    f->show(line);
}

void GUI::Navigation(Entry* f) {
    char x;
    bool back = false;

    // Ridiculous error fix
    gotoXY(0, line + 1);
    setColor(0, 0); printSpace(123);

    // Reset line
    line = 0;
    updateMenu(f);

    while (true) {
        if (_kbhit) {
            x = _getch();
            switch (x) {
            case 13: // enter
            {
                if (line != 0) {
                    if (f->getEntryInList(line - 1)->isFolder())
                        Navigation(f->getEntryInList(line - 1));
                }
                else back = true;

                break;
            }
            case 72: // up
            {
                if (line == 0) {
                    line = f->getListSize();
                }
                else line--;
                line %= f->getListSize() + 1;
                break;
            }
            case 80: // down
            {
                line++;
                line %= f->getListSize() + 1;
                break;
            }
            case 8:
            {
                back = true;
            }
            case 75: //left
            {}
            case 77: // right
            {}

            }
            updateMenu(f);
        }
        if (back) break;
    }
}

void GUI::Function() {

}