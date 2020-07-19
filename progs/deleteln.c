#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {
    setlocale(LC_ALL, "");
    initscr();

	for(int i=0;i<LINES;i++)
		for(int j=0;j<COLS;j++)
			mvinsch(i,j,65+i%26);

	move(5,0);
	deleteln();
    refresh();
    getch();
    endwin();
    return 0;
}
