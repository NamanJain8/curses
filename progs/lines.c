#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {  
    setlocale(LC_ALL, "");
    initscr();

    int maxy = getmaxy(stdscr);
    int maxx = getmaxx(stdscr);

    for(int i=0;i<maxy;i++){
        for(int j=0;j<maxx;j++)
            mvaddstr(i,j,"A");
    }
    mvprintw(0,0,"%d", maxy);
    refresh();
    getch();
    endwin();
    return 0;
}
