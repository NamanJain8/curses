#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {  
    setlocale(LC_ALL, "");
    initscr();
//	start_color();          /* Start color          */
    //init_pair(5, COLOR_RED, COLOR_BLACK);
	cchar_t c;
	wchar_t wstr[] = {'A', 0};
	setcchar(&c, wstr, 0, 0, NULL);

	add_wch(&c);

	short color;
	attr_t att;
	attr_get(&att, &color, NULL);
	cchar_t b;
	move(0,0);
	in_wch(&b);
	mvprintw(5,5,"%x\n",b.attributes);
	mvprintw(6,5,"%x %x\n",att, color);
    refresh();

	getch();
    endwin();
    return 0;
}
