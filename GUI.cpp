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
                /*
                if (line != 0) {
                    if (f->getEntryInList(line - 1)->isFolder())
                        Navigation(f->getEntryInList(line - 1));
                }
                else {
                    back = true;
                    reset();
                }
                */
                EnterFolder(f, back);
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

            // ============= PASSWORD =============
            if (GetKeyState(0x50) & 0x8000) {
                SetPassword(f->getEntryInList(line - 1));
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
    gotoXY(0, 0);

    setColor(15, 0);
}

void GUI::reset() {
    line = 0;
}

string GUI::HidePassword() {
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

string GUI::EnterPassword() {
    string pw, repw;

    while (true) {

        cout << "Password:  ";      pw   = HidePassword();      cout << endl;
        cout << "Re-enter:  ";      repw = HidePassword();      cout << endl;

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

void GUI::EnterFolder(Entry* parent, bool &back) {
    if (line == 0) {
        back = true; 
        reset();
        return;
    }

    string pw;

    Entry* f = parent->getEntryInList(line- 1);

    if (!f->isFolder()) return;

    if (f->isLocked()) {
        clrscr();
        clearBackground();

        pw = EnterPassword();
        
        if (f->checkPassword(pw)) Navigation(f);
        else {
            setColor(4, 0);
            cout << "Error: Invalid password. Access folder denied. ";
            system("pause");
            setColor(15, 0);
        }
    }
    else {
        Navigation(f);
    }
}

void GUI::SetPassword(Entry *f) {
    if (line == 0) return; // Case parent folder

    clrscr();
    clearBackground();

    string pw = EnterPassword();

    if (f->isLocked()) {
        if (f->checkPassword(pw)) f->resetPassword();
        else {
            setColor(4, 0);
            cout << "Error: Invalid password. Reset pasword denied. ";
            system("pause");
            setColor(15, 0);
        }
    }
    else {
        f->setPassword(pw);
    }
}

