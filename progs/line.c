#include <curses.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>
#include <assert.h>
int main() {  
    initscr();
    
    wchar_t c = 0x256c;
    cchar_t cc;
    setcchar(&cc, &c, 0, 0, NULL);
    add_wch(&cc);

    refresh();
    getch();
    endwin();
    return 0;
}
