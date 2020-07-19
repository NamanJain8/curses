#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {  
    setlocale(LC_ALL, "");
    initscr();
    attron(0x0100);
	for(int i=15000;i<15020;i++)
	{
		wchar_t wstr[] = {i, L'\0'};
    		addwstr(wstr);

	}
    attroff(0x0100);
    refresh();
    getch();
    endwin();
    return 0;
}
