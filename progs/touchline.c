#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {
    initscr();

	WINDOW *win1 = newwin(6,6,2,5);
	for(int i=0;i<6;i++)
		for(int j=0;j<6;j++)
			mvwaddch(win1,i,j,65);
	wrefresh(win1);

	WINDOW *win2 = subwin(win1, 3,3,3,6);
	mvwaddstr(win2,0,0,"999");
	mvwaddstr(win2,1,0,"999");
	mvwaddstr(win2,2,0,"99");
	wrefresh(win1);
    refresh();
    getch();
    endwin();
    return 0;
}
