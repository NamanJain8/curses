#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {
    initscr();

	addstr("bbbb");
	refresh();
	WINDOW *win = newwin(6,6,2,5);
	//leaveok(win, true);
	leaveok(stdscr, true);
	waddstr(win, "aaaa");
	wrefresh(win);

	refresh();
    getch();
    endwin();
    return 0;
}
