#include <curses.h>
#include <locale.h>
#include <wchar.h>
#include <assert.h>

int main() {
    initscr();

	WINDOW *win1 = newwin(6,6,2,5);
	WINDOW *win2 = derwin(win1,5,5,1,1);
	WINDOW *win3 = derwin(win2,4,4,1,1);
	WINDOW *win4 = derwin(win3,3,3,1,1);

	//int x = syncok(win3, true);

	waddstr(win1,"a");
//	untouchwin(win1);
	waddstr(win2,"b");
//	untouchwin(win2);
	waddstr(win3, "c");
	waddstr(win4,"d");
//	untouchwin(win4);

//	wsyncup(win3);

	wrefresh(win1);
	wrefresh(win2);
	wrefresh(win3);
	wrefresh(win4);

    getch();
    endwin();
    return 0;
}
