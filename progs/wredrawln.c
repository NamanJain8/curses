#include <curses.h>

int main() {
    initscr();

	WINDOW *win = newwin(4, 10, 5, 6);
	for(int i=0;i<4;i++)
		mvwaddstr(win, i,0,"test");

	untouchwin(win);
	int x = wredrawln(win, 2, 8);

    wrefresh(win);

	printw("%d", x);
	refresh();
    getch();
    endwin();
    return 0;
}
