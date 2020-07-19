#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {
    setlocale(LC_ALL, "");
    initscr();

	cchar_t ch;
	wchar_t wstr[] = {0x3401, L'\0'};
	setcchar(&ch, wstr, 0, 0, NULL);
	add_wch(&ch);
	refresh();
	move(0,1);
	add_wch(&ch);
	add_wch(&ch);
	refresh();

    refresh();
    getch();
    endwin();
    return 0;
}
