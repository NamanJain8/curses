#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {
    setlocale(LC_ALL, "");
    initscr();
	cchar_t ch;
	wchar_t wstr[] = {0x3401, L'\0'};
	setcchar(&ch, wstr, 0, 0, NULL);
	int r = mvaddstr(LINES-1, COLS-1, "a");
	refresh();
	int x,y;
	getyx(stdscr, y,x);
	mvprintw(5,5,"%d %d %d", r, y,x);

    refresh();
    getch();
    endwin();
    return 0;
}
