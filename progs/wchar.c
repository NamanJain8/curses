#include <curses.h>
#include <locale.h>
#include <wchar.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int main() {  
//    setlocale(LC_ALL, "");
    initscr();
    start_color();
	wchar_t wch[] = {0x3401, L'\0'};
	addwstr(wch);
	refresh();
    getch();
    endwin();
    return 0;
}
