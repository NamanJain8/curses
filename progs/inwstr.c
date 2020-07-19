#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {
    setlocale(LC_ALL, "");
    initscr();

	cchar_t ch;
	wchar_t wstr[] = {0x3401, 0x300,  L'\0'};
	setcchar(&ch, wstr, 0, 0, NULL);
	addwstr(wstr);
	move(0,0);

	wchar_t res[10];
	innwstr(res, 5);

	move(5,5);

	printw("%x %x", res[0],res[1]);
    refresh();
    getch();
    endwin();
    return 0;
}
