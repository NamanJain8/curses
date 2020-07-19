#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {
    initscr();

	WINDOW *win = newwin(6,6,2,5);
	for(int i=0;i<6;i++)
		for(int j=0;j<6;j++)
			mvwaddch(win, i,j,49+i);

	wrefresh(win);
	scrollok(win, true);
	wmove(win,3,3);
	scroll(win);

    wrefresh(win);
    getch();
    endwin();
    return 0;
}
