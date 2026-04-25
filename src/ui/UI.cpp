#include "UI.hpp"

#include <ncurses.h>

Mode selectMode(){
    initscr();
    cbreak();
    noecho();

    printw("MODBUS ASCII Terminal\n");
    printw("---------------------\n");
    printw("1. Master\n");
    printw("2. Slave\n\n");
    printw("Select mode: ");
    refresh();

    while(true){
        int ch = getch();
        if (ch == '1'){
            endwin();
            return Mode::MASTER;
        }
        if (ch == '2'){
            endwin();
            return Mode::SLAVE;
        }

        mvprintw(8, 0, "Invalid key, press 1 or 2");
        refresh();
    }
}