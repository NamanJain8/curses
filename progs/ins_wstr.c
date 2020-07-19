#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {
    setlocale(LC_ALL, "");
    initscr();
	wchar_t wstr[] = {'A', 'A', 'A', '\r', 'B', 'B', L'\0'};
	//move(5,5);
	ins_wstr(wstr);
	//addstr("y");
    refresh();
    getch();
    endwin();
    return 0;
}
