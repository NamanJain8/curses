#include <curses.h>
#include <locale.h>
#include <wchar.h>

int main() {  
    setlocale(LC_ALL, "");
    initscr();
    int s = 0x61;     // represents 'A'
    int ns = 0x0305; // represents 

    move(4,0);
    for(int i=0000;i<1000;i++){
    	if(wcwidth(i) == 0){
		ns = i;
		printw("%d ", i);
		wchar_t wstr[] = { s, ns, L'\0'};
    		cchar_t *cc;
    		int x = setcchar(cc, wstr, 0x00, 0, NULL);
    		add_wch(cc);
		printw("%d ", x);
    	}
    }
    refresh();
    getch();
    endwin();
    return 0;
}
