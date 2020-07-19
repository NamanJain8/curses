#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {
    initscr();

	WINDOW *win = newwin(6,6,2,5);
	waddstr(win,"aaaa");
	wrefresh(win);
	refresh();
	getch();

	mvwin(win, 10,10);
	wrefresh(win);

    refresh();
    getch();

	mvwin(win, 20,20);
	wrefresh(win);
	refresh();
	getch();
    endwin();
    return 0;
}
