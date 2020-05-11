#include "GUI.h"

void GUI::setWindows(int height, int width) {
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r); //stores the console's current dimensions

    MoveWindow(console, r.left, r.top, height, width, TRUE); // 1024 width, 576 height
}

void GUI::updateMenu(Entry* f) {
    clrscr();
    
    setColor(0, 10);
    cout << " Path ";
    
    setColor(10, 0);
    cout <<" "<< f->getPath(); printSpace(123 - f->getPath().size() - 7);

    setColor(0, 7);
    cout << " Name"; printSpace(42); cout << " | "; printSpace(16); cout << "Size | Type"; printSpace(6); cout << " | Modified"; printSpace(16); cout << " |  Password " << endl;
    setColor(15, 0);

    f->show(line);
}

/*
   Show list of files/folders in this volume.
   Press hotkeys to implement some funtions:
   - Import:				I
   - Export:				E
   - Delete:				D
   - Set/Remove Password:	P
   - Move down:			    Down
   - Move up:				Up
   - Open a folder:		    Enter
   - Back:					Backspace
   - Exit:					Escape
*/

void GUI::Navigation(Entry* f) {
    // Check if f is nullptr
    if (!f) return;

    char x = 0;
    bool back = false;

    // Ridiculous error fix
    clearBackground();

    // Reset line
    reset();
    updateMenu(f);

    while (true) {
        
        if (_kbhit()) {
            FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

            // ============= ENTER =============
            if (GetKeyState(0x0D) & 0x8000) {
                if (line != 0) {
                    if (f->getEntryInList(line - 1)->isFolder())
                        Navigation(f->getEntryInList(line - 1));
                }
                else {
                    back = true;
                    reset();
                }
            }

            // ============= UP =============
            if (GetKeyState(VK_UP) & 0x8000) {
                if (line == 0) {
                    line = f->getListSize();
                }
                else line--;
                line %= f->getListSize() + 1;
            }

            // ============= DOWN =============
            if (GetKeyState(VK_DOWN) & 0x8000) {
                line++;
                line %= f->getListSize() + 1;
            }

            // ============= BACK =============
            if (GetKeyState(0x08) & 0x8000) {
                back = true;
                reset();
                sleep(50);
            }

            // ============= EXIT =============
            if (GetKeyState(0x1B) & 0x8000) {
                esc = true;
            }


            // Refresh menu
            if (!esc) {
                updateMenu(f);
            }
        }
        

        if (back||esc) break;
    }
}

void GUI::clearBackground() {
    setColor(0, 0);

    printSpace(123); cout << endl;
    printSpace(123);
    gotoXY(0, line + 2);
    printSpace(123); 

    setColor(15, 0);
}

void GUI::reset() {
    line = 0;
}

void GUI::Function() {
    
}

void GUI::sleep(int x) {
    std::this_thread::sleep_for(std::chrono::milliseconds(x));
}