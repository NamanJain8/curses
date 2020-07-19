#include <curses.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string);
int main(int argc, char *argv[])
{	
	setlocale(LC_ALL, "");
	initscr();			/* Start curses mode 		*/
	if(has_colors() == FALSE)
	{	endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
	cchar_t cc;
	wchar_t c = 'a';
	const wchar_t* star = L"0x2605";
	start_color();			/* Start color 			*/
	init_pair(1, COLOR_RED, COLOR_BLACK);
	setcchar(&cc, &c, 0x00000, 1, NULL);
	add_wch(&cc);
	setcchar(&cc, &c, 0x00100, 1, NULL);
	add_wch(&cc);
	setcchar(&cc, &c, 0x00200, 1, NULL);
	add_wch(&cc);
	setcchar(&cc, &c, 0x00400, 1, NULL);
	add_wch(&cc);
	setcchar(&cc, &c, 0x00800, 1, NULL);
	add_wch(&cc);
	setcchar(&cc, &c, 0x01000, 1, NULL);
	add_wch(&cc);
	setcchar(&cc, &c, 0x02000, 1, NULL);
	add_wch(&cc);
	setcchar(&cc, &c, 0x04000, 1, NULL);
	add_wch(&cc);
	wchar_t wstr[] = { 9474, L'\0' };
	setcchar(&cc, wstr, 0x0000, 1, NULL);
	add_wch(&cc);
	attron(COLOR_PAIR(1));
	print_in_middle(stdscr, LINES / 2, 0, 0, "Viola !!! In color ...");
	attroff(COLOR_PAIR(1));
    getch();
	endwin();
}
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string)
{	int length, x, y;
	float temp;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	mvwprintw(win, y, x, "%s", string);
	refresh();
}
