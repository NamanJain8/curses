#include <curses.h>
#include <locale.h>
#include <wchar.h>
#include <assert.h>

int main() {  
    setlocale(LC_ALL, "");
    initscr();

    // delch1   
    move(2,2); 
    addstr("teest");
    refresh();
    getch();
    
    // delch2
    move(2,4);    
    delch();
    refresh();
    getch();
    
    // delch3
    clear();
    move(0,0);
    refresh();
    getch();

    // delch4
    WINDOW* win1 = newwin(6,6,2,5);
    assert(win1 != NULL);
    wrefresh(win1);
    wmove(win1, 1, 0);
    waddstr(win1, "tesst");
    wrefresh(win1);
    getch();
    
    // delch5
    wmove(win1, 1, 3);
    wdelch(win1);
    wrefresh(win1);
    getch();

    endwin();
    return 0;
}
