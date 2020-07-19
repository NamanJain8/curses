#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {
    initscr();

	addstr("test");
	refresh();

	clearok(stdscr, 1);
	clear();

    refresh();
    getch();
    endwin();
    return 0;
}
