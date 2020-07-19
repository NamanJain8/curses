#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {
    setlocale(LC_ALL, "");
    initscr();

	cchar_t ch;
	wchar_t wstr[] = {0x3401, L'\0'};
	setcchar(&ch, wstr, 0, 0, NULL);

	wchar_t bstr[] = {0x8, L'\0'};
	cchar_t back;
	setcchar(&back, bstr, 0, 0, NULL);

	add_wch(&ch);
	add_wch(&ch);
	refresh();
	add_wch(&back);
//	add_wch(&back);
	refresh();
	/*
	echo_wchar(&ch);
	echo_wchar(&ch);
	echo_wchar(&back);
	echo_wchar(&back);
	*/
	int y, x;
	getyx(stdscr, y, x);

	mvprintw(10,10,"%d %d", y, x);

    getch();
    endwin();
    return 0;
}
