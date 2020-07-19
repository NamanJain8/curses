#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {
    setlocale(LC_ALL, "");
    initscr();

	cchar_t ch;
	wchar_t wstr[] = {0x3401, L'\0'};
	setcchar(&ch, wstr, 0, 0, NULL);

	noecho();
	char str[10];
	getnstr(str, 8);
    refresh();
    getch();
    endwin();
    return 0;
}
