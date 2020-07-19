#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {
    initscr();

	nonl();
	addstr("aaa\nbb");

	refresh();
	nl();
	addstr("aaa\nbb");

    refresh();
    getch();
    endwin();
    return 0;
}
