#include <curses.h>
#include <locale.h>
#include <wchar.h>
#include <file.h>

int main() {
    initscr();

	WINDOW *win = newwin(6,6,2,5);
	waddstr(win, "abcd");
	wrefresh(win);
	FILE* fp = open("win.dmp");
	putwin(win, fp);
	close(fp);

    refresh();
    getch();
    endwin();
    return 0;
}
