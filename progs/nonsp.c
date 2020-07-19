#include <curses.h>
#include <locale.h>
#include <wchar.h>
#include <assert.h>

int main() {
    setlocale(LC_ALL, "");
    initscr();
    int s = 0x61;     // represents 'a'
    int ns = 0x0305; // represents COMBINING OVERLINE

    wchar_t wstr[] = { s, ns, L'\0'};
    cchar_t cc;
    int x = setcchar(&cc, wstr, 0x00, 0, NULL);

    mvadd_wch(0,0,&cc);
    curs_set(0);

    cchar_t cc2;
    mvin_wch(0, 0, &cc2);
    mvprintw(4,4, "%x", cc2.vals[1]);

    refresh();
    getch();
    endwin();
    return 0;
}
