#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {  
    setlocale(LC_ALL, "");
    initscr();

    chtype ch = A_STANDOUT | 't';
    echochar(ch);
    getch();
    
    endwin();
    return 0;
}
