#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {
    setlocale(LC_ALL, "");
    initscr();

	cchar_t ch;
	wchar_t wstr[] = {'A', L'\0'};

	mvaddstr(5,5,"xxxxxxxxxx");
	mvaddstr(6,5,"yyyyyyyyyy");
	refresh();

	wchar_t wc[2];
	keypad(stdscr, true);
	mvget_wch(5,7,wc);

    refresh();
    getch();
    endwin();
    return 0;
}
