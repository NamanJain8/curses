#include <curses.h>

int main() {
    initscr();
	mvhline(0,COLS-11,'A', 10);
    refresh();
    getch();
    endwin();
    return 0;
}
