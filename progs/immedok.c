#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {
    setlocale(LC_ALL, "");
    initscr();

	WINDOW *w1 = newwin(6, 6, 0, 0);
	WINDOW *w2 = newwin(6, 6, 10, 0);

	//leaveok(w1, 1);
	//leaveok(w2, 1);
//	immedok(w2, 1);
	waddstr(w1, "Hello");
//	waddstr(w2, "World");
	immedok(w1, 1);
	waddstr(w1, "!");

	getch();
    endwin();
    return 0;
}
