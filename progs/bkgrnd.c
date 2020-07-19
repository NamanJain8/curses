#include <curses.h>
#include <wchar.h>

int main() {
    initscr();

	cchar_t ch,ch2;
	wchar_t wstr[] = {0x61, L'\0'};
	setcchar(&ch, wstr, 0x100, 0, NULL);

	wchar_t wstr2[] = {0x63, L'\0'};
	setcchar(&ch2, wstr2, 0, 0, NULL);
	add_wch(&ch2);
	refresh();
	//bkgd(0x61);
	bkgrnd(&ch);
	refresh();

    getch();
    endwin();
    return 0;
}
