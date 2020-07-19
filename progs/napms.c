#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {
    initscr();

	addstr("haha");
	refresh();
	addstr("lol");
	napms(3000);

    refresh();
    getch();
    endwin();
    return 0;
}
