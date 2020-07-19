#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {
    setlocale(LC_ALL, "");
    initscr();

	cchar_t ch;
	wchar_t wstr[] = {0x3401, L'\0'};
	setcchar(&ch, wstr, 0, 0, NULL);

	WINDOW *win1 = newwin(15, 10, 2, 5);
	for(int i=0;i<10;i++)
		mvwaddstr(win1, i, 0, "0000000000");

	WINDOW *win2 = newwin(6, 5, 3, 6);
	for(int i=0;i<3;i++)
		mvwaddstr(win2, i, 0, "22222");

	wrefresh(win1);
	wrefresh(win2);

	delwin(win2);
	touchwin(win1);
	wrefresh(win1);
	wrefresh(win2);
    refresh();
    getch();
	touchwin(win2);
	wrefresh(win1);
	wrefresh(win2);
	refresh();
	getch();
    endwin();
    return 0;
}
