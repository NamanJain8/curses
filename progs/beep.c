#include <curses.h>
#include <locale.h>
#include <wchar.h>
#include <sys/proc.h>

int main() {
    initscr();

	for(int i=0;i<100;i++){
			sleep(10);
			flash();
	}

    refresh();
    getch();
    endwin();
    return 0;
}
