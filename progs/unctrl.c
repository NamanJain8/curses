#include <curses.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>

int main() {  
    setlocale(LC_ALL, "");
    initscr();

    char *str = (char *) malloc(256);
    str = unctrl('\n');
    printw("%s", str);
    refresh();
    getch();
    
    endwin();
    return 0;
}
