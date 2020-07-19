#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {
    setlocale(LC_ALL, "");
    initscr();
	move(0,0);
	refresh();
	cchar_t ch;
	wchar_t wstr[] = {0x3401, L'\0'};
	setcchar(&ch, wstr, 0, 0, NULL);
	wchar_t str[10];
	addstr("aaa");
	refresh();
	unget_wch(KEY_BACKSPACE);
	get_wch(str);

	refresh();
    getch();
    endwin();
    return 0;
}
