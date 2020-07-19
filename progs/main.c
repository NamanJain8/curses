#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {  
    setlocale(LC_ALL, "");
    initscr();

	noecho();
	for(int i=15000;i<20000;i++)
	{
    		wchar_t wstr[] = { i, L'\0' };
    		addwstr(wstr);
	}
    refresh();
    getch();
    endwin();
    return 0;
}
