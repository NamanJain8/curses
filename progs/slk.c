#include <curses.h>
#include <wchar.h>
#include <assert.h>
#include <locale.h>

int main() {
	setlocale(LC_ALL, "");
	slk_init(0);
	initscr();

	//wchar_t wstr[] = {0x3401, 0x66, 0x3401, 0x66, L'\0'};
	wchar_t wstr[] = {0x3401, 0x66, 0x3401, 0x66, 0x3401, L'\0'};

	assert(wcswidth(wstr, 5) <= 8);

	slk_wset(2, wstr, 1);
	slk_refresh();
	refresh();

	getch();
    endwin();
    return 0;
}
