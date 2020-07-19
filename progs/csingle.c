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
    assert(wcwidth(0x300) == 0);
		wchar_t wch_[] = {0x3401, 0x300, L'\0'};
		//wchar_t wch_[] = {0x61, L'\0'};
		cchar_t cc2;
		setcchar(&cc2, wch_, 0, 0, NULL);
    		mvprintw(9,9, "%d", cc2.elements);
		mvadd_wch(0,0,&cc2);
       /*FILE* fp = fopen("./csingle.dump", "w");
		putwin(stdscr, fp);
		fclose(fp);*/
		
		cchar_t cc;
       // memset(&cc, 0, sizeof(cc));
        for(int i=0;i<5;i++){
            mvprintw(i+10, 4, "%x %x", cc.vals[i], cc2.vals[i]);
        }
        refresh();
		mvin_wch(0,0, &cc);
		mvadd_wch(0,10, &cc);
		for(int i=0;i<5;i++){
			mvprintw(i+2,4, "%x %x", cc.vals[i], cc2.vals[i]);
		}
    		refresh();
		 int y,x;
        //getyx(stdscr, y,x);
		//mvprintw(10,4,"%d",cc2.elements);
        mvprintw(15,10, "%d %d", cc.attributes, cc2.attributes);
		refresh();
		
    getch();
    endwin();
    return 0;
}
