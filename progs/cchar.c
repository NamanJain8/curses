#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {
    setlocale(LC_ALL, "");
    initscr();

	cchar_t ch;
	wchar_t wstr[] = {0x3401, 0x300, L'\0'};
	setcchar(&ch, wstr, 0, 0, NULL);
	add_wch(&ch);

	mvprintw(5,5,"%d", wcslen(wstr));
    refresh();
    getch();
    endwin();
    return 0;
}
