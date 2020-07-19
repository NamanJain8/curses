#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {
    setlocale(LC_ALL, "");
    initscr();

	cchar_t ch;
	wchar_t wstr[] = {'A', L'\0'};

	addwstr(wstr);
	refresh();
	wchar_t wc[2];
	//keypad(stdscr, true);
	get_wch(wc);

	wc[1] = L'\0';
	mvaddwstr(4,4,wc);

    refresh();
    getch();
    endwin();
    return 0;
}
