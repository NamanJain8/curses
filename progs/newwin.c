#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {
    initscr();

	WINDOW *win = newwin(100,200,0,0);
	mvwaddstr(win, 100, 100, "abcd");

    wrefresh(win);
    getch();
    endwin();
    return 0;
}
