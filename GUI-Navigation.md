```cpp
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

    bool ok = 0;
    while (true) {
        // ============= ENTER =============
        if (GetAsyncKeyState(VK_RETURN)) {
            while ((GetAsyncKeyState(VK_RETURN) & 0x8000)) {};

            if (line != 0) {
                if (f->getEntryInList(line - 1)->isFolder())
                    Navigation(f->getEntryInList(line - 1));
            }
            else {
                back = true;
                reset();
            }
            updateMenu(f);
        }

        // ============= UP =============
        if (GetAsyncKeyState(VK_UP)) {
            if (line == 0) {
                line = f->getListSize();
            }
            else line--;
            line %= f->getListSize() + 1;
            updateMenu(f);

            if (ok == 0) {
                int i = 0;
                while ((GetAsyncKeyState(VK_UP) & 0x8000)) {
                    if (i++ >= 300000) {
                        ok = 1;
                        break;
                    }
                };
            }
            else {
                Sleep(25);
                if (!(GetAsyncKeyState(VK_UP) & 0x8000))
                    ok = 0;
            }
        }

        // ============= DOWN =============
        if (GetAsyncKeyState(VK_DOWN)) {
            line++;
            line %= f->getListSize() + 1;
            updateMenu(f);

            if (ok == 0) {
                int i = 0;
                while ((GetAsyncKeyState(VK_DOWN) & 0x8000)) {
                    if (i++ >= 300000) {
                        ok = 1;
                        break;
                    }
                };
            }
            else {
                Sleep(20);
                if (!(GetAsyncKeyState(VK_DOWN) & 0x8000))
                    ok = 0;
            }
        }

        // ============= BACK =============
        if (GetAsyncKeyState(VK_BACK)) {
            while ((GetAsyncKeyState(VK_BACK) & 0x8000)) {};

            back = true;
            reset();
            updateMenu(f);
        }

        // ============= EXIT =============
        if (GetAsyncKeyState(VK_ESCAPE)) {
            esc = true;
        }

        if (back || esc) break;
    }
}
```