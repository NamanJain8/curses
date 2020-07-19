#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {
    setlocale(LC_ALL, "");
    initscr();

	cchar_t ch;
	wchar_t wstr[] = {'j', L'\0'};
	setcchar(&ch, wstr, 0, 0, NULL);
	wchar_t x[] = {0x2518, L'\0'};
	cchar_t ch2;
	setcchar(&ch2, x, 0,0,NULL);
	//border_set(&ch, NULL, &ch, &ch, WACS_ULCORNER, WACS_LRCORNER, WACS_LLCORNER, WACS_URCORNER);
	WINDOW *win = newwin(6,6,10,10); 
	wborder_set(win, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	wrefresh(win);
	mvprintw(5,5,"%d",HAVE_WCHAR);
	mvadd_wch(7,7,&ch2);
	mvadd_wch(8,7,WACS_LRCORNER);
    refresh();
    getch();
    endwin();
    return 0;
}
