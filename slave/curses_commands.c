/*	$NetBSD: curses_commands.c,v 1.9 2019/05/26 07:47:37 blymn Exp $	*/

/*-
 * Copyright 2009 Brett Lymn <blymn@NetBSD.org>
 *
 * All rights reserved.
 *
 * This code has been donated to The NetBSD Foundation by the Author.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. The name of the author may not be used to endorse or promote products
 *    derived from this software withough specific prior written permission
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *
 */

#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <stdarg.h>

#include "slave.h"
#include "curses_commands.h"

void
set_int(int *x, char *arg)
{
    if( sscanf(arg, "%d", x) == 0) {
        report_count(1);
        report_error("BAD ARGUMENT");
        return;
    }
}

void
set_uint(unsigned int *x, char *arg)
{
    if( sscanf(arg, "%d", x) == 0) {
        report_count(1);
        report_error("BAD ARGUMENT");
        return;
    }
}

void
set_short(short *x, char *arg)
{
    if (sscanf(arg, "%hd", x) == 0) {
        report_count(1);
        report_error("BAD ARGUMENT");
        return;
    }
}

void
set_win(WINDOW **x, char *arg)
{
    if (sscanf(arg, "%p", x) == 0) {
        report_count(1);
        report_error("BAD ARGUMENT");
        return;
    }
}

void
set_scrn(SCREEN **x, char *arg)
{
    if (sscanf(arg, "%p", x) == 0) {
        report_count(1);
        report_error("BAD ARGUMENT");
        return;
    }
}


void
cmd_DRAIN(int nargs, char **args)
{
    WINDOW *win;
    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    while (wgetch(win) != ERR);
    report_count(1);
    report_return(OK);
}

void
cmd_addbytes(int nargs, char **args)
{
    int count;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_int(&count, args[1]);

    report_count(1);
    report_return(addbytes(args[0], count));
}


void
cmd_addch(int nargs, char **args)
{
    chtype *ch;

    if (check_arg_count(nargs, 1) == 1)
        return;

    ch = (chtype *) args[0];
    report_count(1);
    report_return(addch(ch[0]));
}


void
cmd_addchnstr(int nargs, char **args)
{
    int count;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_int(&count, args[1]);

    report_count(1);
    report_return(addchnstr((chtype *) args[0], count));
}


void
cmd_addchstr(int nargs, char **args)
{
    if (check_arg_count(nargs, 1) == 1)
        return;

    report_count(1);
    report_return(addchstr((chtype *) args[0]));
}


void
cmd_addnstr(int nargs, char **args)
{
    int count;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_int(&count, args[1]);

    report_count(1);
    report_return(addnstr(args[0], count));
}


void
cmd_addstr(int nargs, char **args)
{
    if (check_arg_count(nargs, 1) == 1)
        return;

    report_count(1);
    report_return(addstr(args[0]));
}


void
cmd_attr_get(int nargs, char **args)
{
    attr_t attrs;
    short colours;
    int retval;

    if (check_arg_count(nargs, 0) == 1)
        return;

    retval = attr_get(&attrs, &colours, NULL);

    /* XXXX - call3 */
    report_count(3);
    report_return(retval);
    report_int(attrs);
    report_int(colours);
}


void
cmd_attr_off(int nargs, char **args)
{
    int attrib;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_int(&attrib, args[0]);

    report_count(1);
    report_return(attr_off(attrib, NULL));
}


void
cmd_attr_on(int nargs, char **args)
{
    int attrib;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_int(&attrib, args[0]);

    report_count(1);
    report_return(attr_on(attrib, NULL));
}


void
cmd_attr_set(int nargs, char **args)
{
    int attrib;
    short pair;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_int(&attrib, args[0]);
    set_short(&pair, args[1]);

    report_count(1);
    report_return(attr_set(attrib, pair, NULL));
}


void
cmd_attroff(int nargs, char **args)
{
    int attrib;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_int(&attrib, args[0]);

    report_count(1);
    report_return(attroff(attrib));
}


void
cmd_attron(int nargs, char **args)
{
    int attrib;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_int(&attrib, args[0]);

    report_count(1);
    report_return(attron(attrib));
}


void
cmd_attrset(int nargs, char **args)
{
    int attrib;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_int(&attrib, args[0]);

    report_count(1);
    report_return(attrset(attrib));
}


void
cmd_bkgd(int nargs, char **args)
{
    chtype *ch;

    if (check_arg_count(nargs, 1) == 1)
        return;

    ch = (chtype *) args[0];
    report_count(1);
    report_return(bkgd(ch[0]));
}


void
cmd_bkgdset(int nargs, char **args)
{
    chtype *ch;

    if (check_arg_count(nargs, 1) == 1)
        return;

    ch = (chtype *) args[0];

    bkgdset(ch[0]); /* returns void */
    report_count(1);
    report_return(OK);
}


void
cmd_border(int nargs, char **args)
{
    int ls, rs, ts, bs, tl, tr, bl, br;

    if (check_arg_count(nargs, 8) == 1)
        return;

    set_int(&ls, args[0]);
    set_int(&rs, args[1]);
    set_int(&ts, args[2]);
    set_int(&bs, args[3]);
    set_int(&tl, args[4]);
    set_int(&tr, args[5]);
    set_int(&bl, args[6]);
    set_int(&br, args[7]);

    report_count(1);
    report_return(border(ls, rs, ts, bs, tl, tr, bl, br));
}


void
cmd_clear(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(clear());
}


void
cmd_clrtobot(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(clrtobot());
}


void
cmd_clrtoeol(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(clrtoeol());
}


void
cmd_color_set(int nargs, char **args)
{
    short colour_pair;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_short(&colour_pair, args[0]);

    report_count(1);
    report_return(color_set(colour_pair, NULL));
}


void
cmd_delch(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(delch());
}


void
cmd_deleteln(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(deleteln());
}


void
cmd_echochar(int nargs, char **args)
{
    chtype *ch;
    if (check_arg_count(nargs, 1) == 1)
        return;

    ch = (chtype *) args[0];
    /* XXX causes refresh */
    report_count(1);
    report_return(echochar(ch[0]));
}


void
cmd_erase(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(erase());
}


void
cmd_getch(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    /* XXX causes refresh */
    report_count(1);
    report_int(getch());
}


void
cmd_getnstr(int nargs, char **args)
{
    int limit;
    char *string;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_int(&limit, args[0]);

    if ((string = malloc(limit + 1)) == NULL) {
        report_count(1);
        report_error("MALLOC_FAILED");
        return;
    }

    /* XXX call2 */
    report_count(2);
    report_return(getnstr(string, limit));
    report_status(string);
    free(string);
}


void
cmd_getstr(int nargs, char **args)
{
    char string[256];

    if (check_arg_count(nargs, 0) == 1)
        return;

    /* XXX call2 */
    report_count(2);
    report_return(getstr(string));
    report_status(string);
}


void
cmd_inch(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_byte(inch());
}


void
cmd_inchnstr(int nargs, char **args)
{
    int limit;
    chtype *string;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_int(&limit, args[0]);

    if ((string = malloc((limit + 1) * sizeof(chtype))) == NULL) {
        report_count(1);
        report_error("MALLOC_FAILED");
        return;
    }

    /* XXX call2 */
    report_count(2);
    report_return(inchnstr(string, limit));
    report_nstr(string);
    free(string);
}


void
cmd_inchstr(int nargs, char **args)
{
    chtype string[256];

    if (check_arg_count(nargs, 0) == 1)
        return;

    /* XXX call2 */
    report_count(2);
    report_return(inchstr(string));
    report_nstr(string);
}


void
cmd_innstr(int nargs, char **args)
{
    int limit;
    char *string;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_int(&limit, args[0]);

    if ((string = malloc(limit + 1)) == NULL) {
        report_count(1);
        report_error("MALLOC_FAILED");
        return;
    }

    /* XXX call2 */
    report_count(2);
    report_int(innstr(string, limit));
    report_status(string);
    free(string);
}


void
cmd_insch(int nargs, char **args)
{
    chtype *ch;
    if (check_arg_count(nargs, 1) == 1)
        return;

    ch = (chtype *) args[0];
    report_count(1);
    report_return(insch(ch[0]));
}


void
cmd_insdelln(int nargs, char **args)
{
    int nlines;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_int(&nlines, args[0]);

    report_count(1);
    report_return(insdelln(nlines));
}


void
cmd_insertln(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(insertln());
}


void
cmd_instr(int nargs, char **args)
{
    char string[256];

    if (check_arg_count(nargs, 0) == 1)
        return;

    /* XXX call2 */
    report_count(2);
    report_return(instr(string));
    report_status(string);
}


void
cmd_move(int nargs, char **args)
{
    int y, x;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);

    report_count(1);
    report_return(move(y, x));
}


void
cmd_refresh(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(refresh());
}


void
cmd_scrl(int nargs, char **args)
{
    int nlines;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_int(&nlines, args[0]);

    report_count(1);
    report_return(scrl(nlines));
}


void
cmd_setscrreg(int nargs, char **args)
{
    int top, bottom;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_int(&top, args[0]);
    set_int(&bottom, args[1]);

    report_count(1);
    report_return(setscrreg(top, bottom));
}


void
cmd_standend(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_int(standend());
}


void
cmd_standout(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_int(standout());
}


void
cmd_timeout(int nargs, char **args)
{
    int tval;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_int(&tval, args[0]);

    timeout(tval); /* void return */
    report_count(1);
    report_return(OK);
}


void
cmd_underscore(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_int(underscore());
}


void
cmd_underend(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_int(underend());
}


void
cmd_waddbytes(int nargs, char **args)
{
    WINDOW *win;
    int count;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&count, args[2]);

    report_count(1);
    report_return(waddbytes(win, args[1], count));
}


void
cmd_waddstr(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_return(waddstr(win, args[1]));
}


void
cmd_mvaddbytes(int nargs, char **args)
{
    int y, x, count;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);
    set_int(&count, args[3]);

    report_count(1);
    report_return(mvaddbytes(y, x, args[2], count));
}


void
cmd_mvaddch(int nargs, char **args)
{
    int y, x;
    chtype *ch;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);
    ch = (chtype *) args[2];

	report_count(1);
    report_return(mvaddch(y, x, ch[0]));
}


void
cmd_mvaddchnstr(int nargs, char **args)
{
    int y, x, count;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);
    set_int(&count, args[3]);

    report_count(1);
    report_return(mvaddchnstr(y, x, (chtype *) args[2], count));
}


void
cmd_mvaddchstr(int nargs, char **args)
{
    int y, x;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);

    report_count(1);
    report_return(mvaddchstr(y, x, (chtype *) args[2]));
}


void
cmd_mvaddnstr(int nargs, char **args)
{
    int y, x, count;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);
    set_int(&count, args[3]);

    report_count(1);
    report_return(mvaddnstr(y, x, args[2], count));
}


void
cmd_mvaddstr(int nargs, char **args)
{
    int y, x;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);

    report_count(1);
    report_return(mvaddstr(y, x, args[2]));
}


void
cmd_mvdelch(int nargs, char **args)
{
    int y, x;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);

    report_count(1);
    report_return(mvdelch(y, x));
}


void
cmd_mvgetch(int nargs, char **args)
{
    int y, x;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);

    report_count(1);
    report_int(mvgetch(y, x));
}


void
cmd_mvgetnstr(int nargs, char **args)
{
    int y, x, count;
    char *string;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);
    set_int(&count, args[2]);

    if ((string = malloc(count + 1)) == NULL) {
        report_count(1);
        report_error("MALLOC_FAILED");
        return;
    }

    /* XXX call2 */
    report_count(2);
    report_return(mvgetnstr(y, x, string, count));
    report_status(string);
    free(string);
}


void
cmd_mvgetstr(int nargs, char **args)
{
    int y, x;
    char string[256];

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);

    /* XXX call2 */
    report_count(2);
    report_return(mvgetstr(y, x, string));
    report_status(string);
}


void
cmd_mvinch(int nargs, char **args)
{
    int y, x;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);

    report_count(1);
    report_byte(mvinch(y, x));
}


void
cmd_mvinchnstr(int nargs, char **args)
{
    int y, x, count;
    chtype *string;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);
    set_int(&count, args[2]);

    if ((string = malloc((count + 1) * sizeof(chtype))) == NULL) {
        report_count(1);
        report_error("MALLOC_FAILED");
        return;
    }

    /* XXX call2 */
    report_count(2);
    report_return(mvinchnstr(y, x, string, count));
    report_nstr(string);
    free(string);
}


void
cmd_mvinchstr(int nargs, char **args)
{
    int y, x;
    chtype string[256];

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);

    /* XXX call2 */
    report_count(2);
    report_return(mvinchstr(y, x, string));
    report_nstr(string);
}


void
cmd_mvinnstr(int nargs, char **args)
{
    int y, x, count;
    char *string;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);
    set_int(&count, args[2]);

    if ((string = malloc(count + 1)) == NULL) {
        report_count(1);
        report_error("MALLOC_FAILED");
        return;
    }

    /* XXX call2 */
    report_count(2);
    report_int(mvinnstr(y, x, string, count));
    report_status(string);
    free(string);
}


void
cmd_mvinsch(int nargs, char **args)
{
    int y, x;
    chtype *ch;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);
    ch = (chtype *) args[2];

    report_count(1);
    report_return(mvinsch(y, x, ch[0]));
}


void
cmd_mvinstr(int nargs, char **args)
{
    char string[256];
    int y, x;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);

    report_count(2);
    report_return(mvinstr(y, x, string));
    report_status(string);
}



void
cmd_mvwaddbytes(int nargs, char **args)
{
    int y, x, count;
    WINDOW *win;

    if (check_arg_count(nargs, 5) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);
    set_int(&count, args[4]);

    report_count(1);
    report_return(mvwaddbytes(win, y, x, args[3], count));
}


void
cmd_mvwaddch(int nargs, char **args)
{
    int y, x;
    WINDOW *win;
    chtype *ch;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);
    ch = (chtype *) args[3];

    report_count(1);
    report_return(mvwaddch(win, y, x, ch[0]));
}


void
cmd_mvwaddchnstr(int nargs, char **args)
{
    int y, x, count;
    WINDOW *win;

    if (check_arg_count(nargs, 5) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);
    set_int(&count, args[4]);

    report_count(1);
    report_return(mvwaddchnstr(win, y, x, (chtype *) args[3], count));
}


void
cmd_mvwaddchstr(int nargs, char **args)
{
    int y, x;
    WINDOW *win;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);

    report_count(1);
    report_return(mvwaddchstr(win, y, x, (chtype *) args[3]));
}


void
cmd_mvwaddnstr(int nargs, char **args)
{
    int y, x, count;
    WINDOW *win;

    if (check_arg_count(nargs, 5) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);
    set_int(&count, args[4]);

    report_count(1);
    report_return(mvwaddnstr(win, y, x, args[3], count));
}


void
cmd_mvwaddstr(int nargs, char **args)
{
    int y, x;
    WINDOW *win;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);
    report_count(1);
    report_return(mvwaddstr(win, y, x, args[3]));
}


void
cmd_mvwdelch(int nargs, char **args)
{
    int y, x;
    WINDOW *win;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);

    report_count(1);
    report_return(mvwdelch(win, y, x));
}


void
cmd_mvwgetch(int nargs, char **args)
{
    int y, x;
    WINDOW *win;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);

    /* XXX - implicit refresh */
    report_count(1);
    report_int(mvwgetch(win, y, x));
}


void
cmd_mvwgetnstr(int nargs, char **args)
{
    int y, x, count;
    char *string;
    WINDOW *win;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);
    set_int(&count, args[3]);

    if ((string = malloc(count + 1)) == NULL) {
        report_count(1);
        report_error("MALLOC_FAILED");
        return;
    }

    /* XXX call2 */
    report_count(2);
    report_return(mvwgetnstr(win, y, x, string, count));
    report_status(string);
    free(string);
}


void
cmd_mvwgetstr(int nargs, char **args)
{
    int y, x;
    WINDOW *win;
    char string[256];

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);

    /* XXX - call2 */
    report_count(2);
    report_return(mvwgetstr(win, y, x, string));
    report_status(string);
}


void
cmd_mvwinch(int nargs, char **args)
{
    int y, x;
    WINDOW *win;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);

    report_count(1);
    report_byte(mvwinch(win, y, x));
}


void
cmd_mvwinsch(int nargs, char **args)
{
    int y, x;
    WINDOW *win;
    chtype *ch;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);
    ch = (chtype *) args[3];

	report_count(1);
    report_return(mvwinsch(win, y, x, ch[0]));
}


void
cmd_assume_default_colors(int nargs, char **args)
{
    short fore, back;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_short(&fore, args[0]);
    set_short(&back, args[1]);

    report_count(1);
    report_return(assume_default_colors(fore, back));
}


void
cmd_baudrate(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_int(baudrate());
}


void
cmd_beep(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(beep());
}


void
cmd_box(int nargs, char **args)
{
    WINDOW *win;
    chtype *vertical, *horizontal;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    vertical = (chtype *) args[1];
    horizontal = (chtype *) args[2];

	report_count(1);
    report_return(box(win, vertical[0], horizontal[0]));
}


void
cmd_can_change_color(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_int(can_change_color());
}


void
cmd_cbreak(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(cbreak());
}


void
cmd_clearok(int nargs, char **args)
{
    WINDOW *win;
    int flag;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&flag, args[1]);

    report_count(1);
    report_return(clearok(win, flag));
}


void
cmd_color_content(int nargs, char **args)
{
    short colour, red, green, blue;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_short(&colour, args[0]);

    /* XXX - call4 */
    report_count(4);
    report_return(color_content(colour, &red, &green, &blue));
    report_int(red);
    report_int(green);
    report_int(blue);
}


void
cmd_copywin(int nargs, char **args)
{
    int sminrow, smincol, dminrow, dmincol, dmaxrow, dmaxcol, ovlay;
    WINDOW *source, *destination;

    if (check_arg_count(nargs, 9) == 1)
        return;

    set_win(&source, args[0]);
    set_win(&destination, args[1]);
    set_int(&sminrow, args[2]);
    set_int(&smincol, args[3]);
    set_int(&dminrow, args[4]);
    set_int(&dmincol, args[5]);
    set_int(&dmaxrow, args[6]);
    set_int(&dmaxcol, args[7]);
    set_int(&ovlay, args[8]);

    report_count(1);
    report_return(copywin(source, destination, sminrow, smincol, dminrow,
                  dmincol, dmaxrow, dmaxcol, ovlay));
}


void
cmd_curs_set(int nargs, char **args)
{
    int vis;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_int(&vis, args[0]);

    report_count(1);
    report_int(curs_set(vis));
}


void
cmd_def_prog_mode(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(def_prog_mode());
}


void
cmd_def_shell_mode(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(def_shell_mode());
}


void
cmd_define_key(int nargs, char **args)
{
    int symbol;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_int(&symbol, args[1]);

    report_count(1);
    report_return(define_key(args[0], symbol));
}


void
cmd_delay_output(int nargs, char **args)
{
    int dtime;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_int(&dtime, args[0]);

    report_count(1);
    report_return(delay_output(dtime));
}


void
cmd_delscreen(int nargs, char **args)
{
    SCREEN *scrn;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_scrn(&scrn, args[0]);

    delscreen(scrn); /* void return */
    report_count(1);
    report_return(OK);
}


void
cmd_delwin(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_return(delwin(win));
}


void
cmd_derwin(int nargs, char **args)
{
    int lines, cols, y, x;
    WINDOW *win;

    if (check_arg_count(nargs, 5) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&lines, args[1]);
    set_int(&cols, args[2]);
    set_int(&y, args[3]);
    set_int(&x, args[4]);

	report_count(1);
    report_ptr(derwin(win, lines, cols, y, x));
}


void
cmd_dupwin(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_ptr(dupwin(win));
}


void
cmd_doupdate(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    /* XXX - implicit refresh */
    report_count(1);
    report_return(doupdate());
}


void
cmd_echo(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(echo());
}


void
cmd_endwin(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(endwin());
}


void
cmd_erasechar(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_int(erasechar());
}


void
cmd_flash(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(flash());
}


void
cmd_flushinp(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(flushinp());
}


void
cmd_flushok(int nargs, char **args)
{
    int flag;
    WINDOW *win;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&flag, args[1]);

    report_count(1);
    report_return(flushok(win, flag));
}


void
cmd_fullname(int nargs, char **args)
{
    char string[256];

    if (check_arg_count(nargs, 1) == 1)
        return;

    /* XXX - call2 */
    report_count(2);
    report_status(fullname(args[0], string));
    report_status(string);
}


void
cmd_getattrs(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_int(getattrs(win));
}


void
cmd_getbkgd(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_byte(getbkgd(win));
}


void
cmd_getcury(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_int(getcury(win));
}


void
cmd_getcurx(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_int(getcurx(win));
}


void
cmd_getyx(int nargs, char **args)
{
    WINDOW *win;
    int y, x;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    getyx(win, y, x);
    report_count(2);
    report_int(y);
    report_int(x);
}


void
cmd_getbegy(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_int(getbegy(win));
}


void
cmd_getbegx(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_int(getbegx(win));
}


void
cmd_getmaxy(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_int(getmaxy(win));
}


void
cmd_getmaxx(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_int(getmaxx(win));
}


void
cmd_getpary(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_int(getpary(win));
}


void
cmd_getparx(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_int(getparx(win));
}


void
cmd_getparyx(int nargs, char **args)
{
    WINDOW *win;
    int y, x;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(2);
    getparyx(win, y, x);
    report_int(y);
    report_int(x);
}

void
cmd_getmaxyx(int nargs, char **args)
{
    WINDOW *win;
    int y, x;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    getmaxyx(win, y, x);
    report_count(2);
    report_int(y);
    report_int(x);
}

void
cmd_getbegyx(int nargs, char **args)
{
    WINDOW *win;
    int y, x;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    getbegyx(win, y, x);
    report_count(2);
    report_int(y);
    report_int(x);
}

void
cmd_setsyx(int nargs, char **args)
{
    int y, x;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);

    report_count(1);
    setsyx(y, x);
    report_return(OK);
}

void
cmd_getsyx(int nargs, char **args)
{
    int y, x;

    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(3);
    getsyx(y, x);
    report_return(OK);
    report_int(y);
    report_int(x);
}

void
cmd_gettmode(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(gettmode());
}


void
cmd_getwin(int nargs, char **args)
{
    FILE *fp;

    if (check_arg_count(nargs, 1) == 1)
        return;

    if ((fp = fopen(args[0], "r")) == NULL) {
        report_count(1);
        report_error("BAD FILE_ARGUMENT");
        return;
    }

    report_count(1);
    report_ptr(getwin(fp));
    fclose(fp);
}


void
cmd_halfdelay(int nargs, char **args)
{
    int ms;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_int(&ms, args[0]);

    report_count(1);
    report_return(halfdelay(ms));
}


void
cmd_has_colors(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_int(has_colors());
}


void
cmd_has_ic(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_int(has_ic());
}


void
cmd_has_il(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_int(has_il());
}


void
cmd_hline(int nargs, char **args)
{
    int count;
    chtype *ch;

    if (check_arg_count(nargs, 2) == 1)
        return;

    ch = (chtype *) args[0];
    set_int(&count, args[1]);

    report_count(1);
    report_return(hline(ch[0], count));
}


void
cmd_idcok(int nargs, char **args)
{
    int flag;
    WINDOW *win;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&flag, args[1]);

    report_count(1);
    report_return(idcok(win, flag));
}


void
cmd_idlok(int nargs, char **args)
{
    int flag;
    WINDOW *win;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&flag, args[1]);

    report_count(1);
    report_return(idlok(win, flag));
}


void
cmd_init_color(int nargs, char **args)
{
    short colour, red, green, blue;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_short(&colour, args[0]);
    set_short(&red, args[1]);
    set_short(&green, args[2]);
    set_short(&blue, args[3]);

    report_count(1);
    report_return(init_color(colour, red, green, blue));
}


void
cmd_init_pair(int nargs, char **args)
{
    short pair, fore, back;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_short(&pair, args[0]);
    set_short(&fore, args[1]);
    set_short(&back, args[2]);

    report_count(1);
    report_return(init_pair(pair, fore, back));
}


void
cmd_initscr(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_ptr(initscr());
}


void
cmd_intrflush(int nargs, char **args)
{
    int flag;
    WINDOW *win;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&flag, args[1]);

    report_count(1);
    report_return(intrflush(win, flag));
}


void
cmd_isendwin(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_int(isendwin());
}


void
cmd_is_linetouched(int nargs, char **args)
{
    int line;
    WINDOW *win;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&line, args[1]);

	report_count(1);
    report_int(is_linetouched(win, line));
}


void
cmd_is_wintouched(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_int(is_wintouched(win));
}


void
cmd_keyok(int nargs, char **args)
{
    int keysym, flag;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_int(&keysym, args[0]);
    set_int(&flag, args[1]);

    report_count(1);
    report_return(keyok(keysym, flag));
}


void
cmd_keypad(int nargs, char **args)
{
    int flag;
    WINDOW *win;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&flag, args[1]);

    report_count(1);
    report_return(keypad(win, flag));
}

void
cmd_is_keypad(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_int(is_keypad(win));
}

void
cmd_keyname(int nargs, char **args)
{
    unsigned int key;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_uint(&key, args[0]);

    report_count(1);
    report_status(keyname(key));
}


void
cmd_killchar(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_int(killchar());
}


void
cmd_leaveok(int nargs, char **args)
{
    int flag;
    WINDOW *win;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&flag, args[1]);

    report_count(1);
    report_return(leaveok(win, flag));
}

void
cmd_is_leaveok(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_int(is_leaveok(win));
}

void
cmd_meta(int nargs, char **args)
{
    int flag;
    WINDOW *win;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&flag, args[1]);

    report_count(1);
    report_return(meta(win, flag));
}


void
cmd_mvcur(int nargs, char **args)
{
    int oldy, oldx, y, x;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_int(&oldy, args[0]);
    set_int(&oldx, args[1]);
    set_int(&y, args[2]);
    set_int(&x, args[3]);

    report_count(1);
    report_return(mvcur(oldy, oldx, y, x));
}


void
cmd_mvderwin(int nargs, char **args)
{
    int y, x;
    WINDOW *win;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);

	report_count(1);
    report_return(mvderwin(win, y, x));
}


void
cmd_mvhline(int nargs, char **args)
{
    int y, x, n;
    chtype *ch;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);
    ch = (chtype *) args[2];
    set_int(&n, args[3]);

    report_count(1);
    report_return(mvhline(y, x, ch[0], n));
}


void
cmd_mvprintw(int nargs, char **args)
{
    int y, x;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);

    report_count(1);
    report_return(mvprintw(y, x, args[2], args[3]));
}


void
cmd_mvscanw(int nargs, char **args)
{
    int y, x;
    char string[256];

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);

    /* XXX - call2 */
    report_count(2);
    report_return(mvscanw(y, x, args[2], &string));
    report_status(string);
}


void
cmd_mvvline(int nargs, char **args)
{
    int y, x, n;
    chtype *ch;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);
    ch = (chtype *) args[2];
    set_int(&n, args[3]);

    report_count(1);
    report_return(mvvline(y, x, ch[0], n));
}


void
cmd_mvwhline(int nargs, char **args)
{
    int y, x, n;
    chtype *ch;
    WINDOW *win;

    if (check_arg_count(nargs, 5) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);
    ch = (chtype *) args[3];
    set_int(&n, args[4]);

    report_count(1);
    report_return(mvwhline(win, y, x, ch[0], n));
}


void
cmd_mvwvline(int nargs, char **args)
{
    int y, x, n;
    WINDOW *win;
    chtype *ch;

    if (check_arg_count(nargs, 5) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);
    ch = (chtype *) args[3];
    set_int(&n, args[4]);

    report_count(1);
    report_return(mvwvline(win, y, x, ch[0], n));
}


void
cmd_mvwin(int nargs, char **args)
{
    int y, x;
    WINDOW *win;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);

    report_count(1);
    report_return(mvwin(win, y, x));
}


void
cmd_mvwinchnstr(int nargs, char **args)
{
    int y, x, count;
    chtype *string;
    WINDOW *win;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);
    set_int(&count, args[3]);

    if ((string = malloc((count + 1) * sizeof(chtype))) == NULL) {
        report_count(1);
        report_error("MALLOC_FAILED");
        return;
    }

    /* XXX call2 */
    report_count(2);
    report_return(mvwinchnstr(win, y, x, string, count));
    report_nstr(string);
    free(string);
}


void
cmd_mvwinchstr(int nargs, char **args)
{
    int y, x;
    chtype string[256];
    WINDOW *win;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);

    /* XXX call2 */
    report_count(2);
    report_return(mvwinchstr(win, y, x, string));
    report_nstr(string);
}


void
cmd_mvwinnstr(int nargs, char **args)
{
    int y, x, count;
    char *string;
    WINDOW *win;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);
    set_int(&count, args[3]);

    if ((string = malloc(count + 1)) == NULL) {
        report_count(1);
        report_error("MALLOC_FAILED");
        return;
    }

    /* XXX call2 */
    report_count(2);
    report_int(mvwinnstr(win, y, x, string, count));
    report_status(string);
    free(string);
}


void
cmd_mvwinstr(int nargs, char **args)
{
    int y, x;
    char string[256];
    WINDOW *win;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);

    /* XXX call2 */
    report_count(2);
    report_return(mvwinstr(win, y, x, string));
    report_status(string);
}


void
cmd_mvwprintw(int nargs, char **args)
{
    int y, x;
    WINDOW *win;

    if (check_arg_count(nargs, 5) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);

	report_count(1);
    report_return(mvwprintw(win, y, x, args[3], args[4]));
}


void
cmd_mvwscanw(int nargs, char **args)
{
    int y, x;
    WINDOW *win;
    char string[256];

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);

    /* XXX - call2 */
    report_count(2);
    report_int(mvwscanw(win, y, x, args[3], &string));
    report_status(string);
}


void
cmd_napms(int nargs, char **args)
{
    int naptime;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_int(&naptime, args[0]);

    report_count(1);
    report_return(napms(naptime));
}


void
cmd_newpad(int nargs, char **args)
{
    int y, x;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);

    report_count(1);
    report_ptr(newpad(y, x));
}


void
cmd_newterm(int nargs, char **args)
{
    FILE *in, *out;

    if (check_arg_count(nargs, 3) == 1)
        return;

    if ((in = fopen(args[1], "rw")) == NULL) {
        report_count(1);
        report_error("BAD FILE_ARGUMENT");
        return;
    }

    if ((out = fopen(args[2], "rw")) == NULL) {
        report_count(1);
        report_error("BAD FILE_ARGUMENT");
        return;
    }

    report_count(1);
    report_ptr(newterm(args[0], out, in));
}


void
cmd_newwin(int nargs, char **args)
{
    int lines, cols, begin_y, begin_x;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_int(&lines, args[0]);
    set_int(&cols, args[1]);
    set_int(&begin_y, args[2]);
    set_int(&begin_x, args[3]);

    report_count(1);
    report_ptr(newwin(lines, cols, begin_y, begin_x));
}


void
cmd_nl(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(nl());
}


void
cmd_no_color_attributes(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_int(no_color_attributes());
}


void
cmd_nocbreak(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(nocbreak());
}


void
cmd_nodelay(int nargs, char **args)
{
    int flag;
    WINDOW *win;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&flag, args[1]);

    report_count(1);
    report_return(nodelay(win, flag));
}


void
cmd_noecho(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(noecho());
}


void
cmd_nonl(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(nonl());
}


void
cmd_noqiflush(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    noqiflush();
    report_count(1);
    report_return(OK); /* fake a return, the call returns void */
}


void
cmd_noraw(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(noraw());
}


void
cmd_notimeout(int nargs, char **args)
{
    int flag;
    WINDOW *win;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&flag, args[1]);

    report_count(1);
    report_return(notimeout(win, flag));
}


void
cmd_overlay(int nargs, char **args)
{
    WINDOW *source, *dest;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&source, args[0]);
    set_win(&dest, args[1]);

    report_count(1);
    report_return(overlay(source, dest));
}


void
cmd_overwrite(int nargs, char **args)
{
    WINDOW *source, *dest;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&source, args[0]);
    set_win(&dest, args[1]);

    report_count(1);
    report_return(overwrite(source, dest));
}


void
cmd_pair_content(int nargs, char **args)
{
    short pair, fore, back;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_short(&pair, args[0]);

    /* XXX - call3 */
    report_count(3);
    report_return(pair_content(pair, &fore, &back));
    report_int(fore);
    report_int(back);
}


void
cmd_pechochar(int nargs, char **args)
{
    chtype *ch;
    WINDOW *pad;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&pad, args[0]);
    ch = (chtype *) args[1];

    report_count(1);
    report_return(pechochar(pad, ch[0]));
}


void
cmd_pnoutrefresh(int nargs, char **args)
{
    int pbeg_y, pbeg_x, sbeg_y, sbeg_x, smax_y, smax_x;
    WINDOW *pad;

    if (check_arg_count(nargs, 7) == 1)
        return;

    set_win(&pad, args[0]);
    set_int(&pbeg_y, args[1]);
    set_int(&pbeg_x, args[2]);
    set_int(&sbeg_y, args[3]);
    set_int(&sbeg_x, args[4]);
    set_int(&smax_y, args[5]);
    set_int(&smax_x, args[6]);

    report_count(1);
    report_return(pnoutrefresh(pad, pbeg_y, pbeg_x, sbeg_y, sbeg_x, smax_y,
                   smax_x));
}


void
cmd_prefresh(int nargs, char **args)
{
    int pbeg_y, pbeg_x, sbeg_y, sbeg_x, smax_y, smax_x;
    WINDOW *pad;

    if (check_arg_count(nargs, 7) == 1)
        return;

    set_win(&pad, args[0]);
    set_int(&pbeg_y, args[1]);
    set_int(&pbeg_x, args[2]);
    set_int(&sbeg_y, args[3]);
    set_int(&sbeg_x, args[4]);
    set_int(&smax_y, args[5]);
    set_int(&smax_x, args[6]);

    /* XXX causes refresh */
    report_count(1);
    report_return(prefresh(pad, pbeg_y, pbeg_x, sbeg_y, sbeg_x, smax_y,
                   smax_x));

}


void
cmd_printw(int nargs, char **args)
{
    if (check_arg_count(nargs, 2) == 1)
        return;

    report_count(1);
    report_return(printw(args[0], args[1]));
}


void
cmd_putwin(int nargs, char **args)
{
    FILE *fp;
    WINDOW *win;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);

    if ((fp = fopen(args[1], "w")) == NULL) {
        report_count(1);
        report_error("BAD FILE_ARGUMENT");
        return;
    }

    report_count(1);
    report_return(putwin(win, fp));
    fclose(fp);
}


void
cmd_qiflush(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    qiflush();
    report_count(1);
    report_return(OK); /* fake a return because call returns void */
}


void
cmd_raw(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(raw());
}


void
cmd_redrawwin(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_return(redrawwin(win));
}


void
cmd_reset_prog_mode(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(reset_prog_mode());
}


void
cmd_reset_shell_mode(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(reset_shell_mode());
}


void
cmd_resetty(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(resetty());
}


void
cmd_resizeterm(int nargs, char **args)
{
    int rows, cols;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_int(&rows, args[0]);
    set_int(&cols, args[1]);

    report_count(1);
    report_return(resizeterm(rows, cols));
}


void
cmd_savetty(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(savetty());
}


void
cmd_scanw(int nargs, char **args)
{
    char string[256];

    if (check_arg_count(nargs, 0) == 1)
        return;

    /* XXX call2 */
    report_count(2);
    report_return(scanw("%s", string));
    report_status(string);
}


void
cmd_scroll(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_return(scroll(win));
}


void
cmd_scrollok(int nargs, char **args)
{
    WINDOW *win;
    int flag;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&flag, args[1]);

    report_count(1);
    report_return(scrollok(win, flag));
}


void
cmd_setterm(int nargs, char **args)
{
    if (check_arg_count(nargs, 1) == 1)
        return;

    report_count(1);
    report_return(setterm(args[0]));
}


void
cmd_set_term(int nargs, char **args)
{
    SCREEN *scrn;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_scrn(&scrn, args[0]);

    report_count(1);
    report_ptr(set_term(scrn));
}


void
cmd_start_color(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(start_color());
}


void
cmd_subpad(int nargs, char **args)
{
    WINDOW *pad;
    int lines, cols, begin_y, begin_x;

    if (check_arg_count(nargs, 5) == 1)
        return;

    set_win(&pad, args[0]);
    set_int(&lines, args[1]);
    set_int(&cols, args[2]);
    set_int(&begin_y, args[3]);
    set_int(&begin_x, args[4]);

    report_count(1);
    report_ptr(subpad(pad, lines, cols, begin_y, begin_x));
}


void
cmd_subwin(int nargs, char **args)
{
    WINDOW *win;
    int lines, cols, begin_y, begin_x;

    if (check_arg_count(nargs, 5) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&lines, args[1]);
    set_int(&cols, args[2]);
    set_int(&begin_y, args[3]);
    set_int(&begin_x, args[4]);

    report_count(1);
    report_ptr(subwin(win, lines, cols, begin_y, begin_x));
}


void
cmd_termattrs(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_int(termattrs());
}


void
cmd_term_attrs(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_int(term_attrs());
}


void
cmd_touchline(int nargs, char **args)
{
    WINDOW *win;
    int start, count;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&start, args[1]);
    set_int(&count, args[2]);

    report_count(1);
    report_return(touchline(win, start, count));
}


void
cmd_touchoverlap(int nargs, char **args)
{
    WINDOW *win1, *win2;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win1, args[0]);
    set_win(&win2, args[1]);

    report_count(1);
    report_return(touchoverlap(win1, win2));
}


void
cmd_touchwin(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_return(touchwin(win));
}


void
cmd_ungetch(int nargs, char **args)
{
    int ch;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_int(&ch, args[0]);

    report_count(1);
    report_return(ungetch(ch));
}


void
cmd_untouchwin(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_return(untouchwin(win));
}


void
cmd_use_default_colors(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(use_default_colors());
}


void
cmd_vline(int nargs, char **args)
{
    int count;
    chtype *ch;

    if (check_arg_count(nargs, 2) == 1)
        return;

    ch = (chtype *) args[0];
    set_int(&count, args[1]);

    report_count(1);
    report_return(vline(ch[0], count));
}


static int
internal_vw_printw(WINDOW *win, char *arg1, ...)
{
    va_list va;
    int rv;

    va_start(va, arg1);
    rv = vw_printw(win, arg1, va);
    va_end(va);

    return rv;
}

void
cmd_vw_printw(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_return(internal_vw_printw(win, args[1], args[2]));
}


static int
internal_vw_scanw(WINDOW *win, char *arg1, ...)
{
    va_list va;
    int rv;

    va_start(va, arg1);
    rv = vw_scanw(win, arg1, va);
    va_end(va);

    return rv;
}

void
cmd_vw_scanw(int nargs, char **args)
{
    WINDOW *win;
    char string[256];

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);

    /* XXX - call2 */
    report_count(2);
    report_int(internal_vw_scanw(win, args[1], string));
    report_status(string);
}


void
cmd_vwprintw(int nargs, char **args)
{
    cmd_vw_printw(nargs, args);
}


void
cmd_vwscanw(int nargs, char **args)
{
    cmd_vw_scanw(nargs, args);
}


void
cmd_waddch(int nargs, char **args)
{
    WINDOW *win;
    chtype *ch;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    ch = (chtype *) args[1];

    report_count(1);
    report_return(waddch(win, ch[0]));
}


void
cmd_waddchnstr(int nargs, char **args)
{
    WINDOW *win;
    int count;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&count, args[2]);

    report_count(1);
    report_return(waddchnstr(win, (chtype *) args[1], count));
}


void
cmd_waddchstr(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_return(waddchstr(win, (chtype *) args[1]));
}


void
cmd_waddnstr(int nargs, char **args)
{
    WINDOW *win;
    int count;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&count, args[2]);

    report_count(1);
    report_return(waddnstr(win, args[1], count));

}


void
cmd_wattr_get(int nargs, char **args)
{
    WINDOW *win;
    int attr;
    short pair;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    /* XXX - call3 */
    report_count(3);
    report_return(wattr_get(win, &attr, &pair, NULL));
    report_int(attr);
    report_int(pair);
}


void
cmd_wattr_off(int nargs, char **args)
{
    WINDOW *win;
    int attr;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&attr, args[1]);

    report_count(1);
    report_return(wattr_off(win, attr, NULL));
}


void
cmd_wattr_on(int nargs, char **args)
{
    WINDOW *win;
    int attr;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&attr, args[1]);

    report_count(1);
    report_return(wattr_on(win, attr, NULL));
}


void
cmd_wattr_set(int nargs, char **args)
{
    WINDOW *win;
    int attr;
    short pair;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&attr, args[1]);
    set_short(&pair, args[2]);

    report_count(1);
    report_return(wattr_set(win, attr, pair, NULL));
}


void
cmd_wattroff(int nargs, char **args)
{
    WINDOW *win;
    int attr;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&attr, args[1]);

    report_count(1);
    report_return(wattroff(win, attr));
}


void
cmd_wattron(int nargs, char **args)
{
    WINDOW *win;
    int attr;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&attr, args[1]);

    report_count(1);
    report_return(wattron(win, attr));
}


void
cmd_wattrset(int nargs, char **args)
{
    WINDOW *win;
    int attr;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&attr, args[1]);

    report_count(1);
    report_return(wattrset(win, attr));
}


void
cmd_wbkgd(int nargs, char **args)
{
    WINDOW *win;
    chtype *ch;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);

    ch = (chtype *) args[1];
    report_count(1);
    report_return(wbkgd(win, ch[0]));
}


void
cmd_wbkgdset(int nargs, char **args)
{
    WINDOW *win;
    chtype *ch;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    ch = (chtype *) args[1];

    wbkgdset(win, *ch); /* void return */
    report_count(1);
    report_return(OK);
}


void
cmd_wborder(int nargs, char **args)
{
    WINDOW *win;
    int ls, rs, ts, bs, tl, tr, bl, br;

    if (check_arg_count(nargs, 9) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&ls, args[1]);
    set_int(&rs, args[2]);
    set_int(&ts, args[3]);
    set_int(&bs, args[4]);
    set_int(&tl, args[5]);
    set_int(&tr, args[6]);
    set_int(&bl, args[7]);
    set_int(&br, args[8]);

    report_count(1);
    report_return(wborder(win, ls, rs, ts, bs, tl, tr, bl, br));
}


void
cmd_wclear(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_return(wclear(win));
}


void
cmd_wclrtobot(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_return(wclrtobot(win));
}


void
cmd_wclrtoeol(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_return(wclrtoeol(win));

}


void
cmd_wcolor_set(int nargs, char **args)
{
    WINDOW *win;
    short pair;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    set_short(&pair, args[1]);

    report_count(1);
    report_return(wcolor_set(win, pair, NULL));
}


void
cmd_wdelch(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_return(wdelch(win));
}


void
cmd_wdeleteln(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_return(wdeleteln(win));

}


void
cmd_wechochar(int nargs, char **args)
{
    WINDOW *win;
    int ch;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&ch, args[1]);

    report_count(1);
    report_return(wechochar(win, ch));
}


void
cmd_werase(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_return(werase(win));
}


void
cmd_wgetch(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_int(wgetch(win));
}


void
cmd_wgetnstr(int nargs, char **args)
{
    WINDOW *win;
    int count;
    char string[256];

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&count, args[1]);

    /* XXX - call2 */
    report_count(2);
    report_return(wgetnstr(win, string, count));
    report_status(string);
}


void
cmd_wgetstr(int nargs, char **args)
{
    WINDOW *win;
    char string[256];


    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    string[0] = '\0';

    report_count(2);
    report_return(wgetstr(win, string));
    report_status(string);
}


void
cmd_whline(int nargs, char **args)
{
    WINDOW *win;
    int count;
    chtype *ch;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    ch = (chtype *) args[1];
    set_int(&count, args[2]);

    report_count(1);
    report_return(whline(win, ch[0], count));
}


void
cmd_winch(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_byte(winch(win));
}


void
cmd_winchnstr(int nargs, char **args)
{
    WINDOW *win;
    chtype string[256];
    int count;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&count, args[1]);

    /* XXX - call2 */
    report_count(2);
    report_return(winchnstr(win, string, count));
    report_nstr(string);
}


void
cmd_winchstr(int nargs, char **args)
{
    WINDOW *win;
    chtype string[256];

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    /* XXX - call2 */
    report_count(2);
    report_return(winchstr(win, string));
    report_nstr(string);
}


void
cmd_winnstr(int nargs, char **args)
{
    WINDOW *win;
    char string[256];
    int count;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&count, args[1]);

    /* XXX - call2 */
    report_count(2);
    report_int(winnstr(win, string, count));
    report_status(string);
}


void
cmd_winsch(int nargs, char **args)
{
    WINDOW *win;
    chtype *ch;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    ch = (chtype *) args[1];

    report_count(1);
    report_return(winsch(win, ch[0]));
}


void
cmd_winsdelln(int nargs, char **args)
{
    WINDOW *win;
    int count;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&count, args[1]);

    report_count(1);
    report_return(winsdelln(win, count));
}


void
cmd_winsertln(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_return(winsertln(win));
}


void
cmd_winstr(int nargs, char **args)
{
    WINDOW *win;
    char string[256];

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    /* XXX - call2 */
    report_count(2);
    report_return(winstr(win, string));
    report_status(string);
}


void
cmd_wmove(int nargs, char **args)
{
    WINDOW *win;
    int y, x;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);

    report_count(1);
    report_return(wmove(win, y, x));
}


void
cmd_wnoutrefresh(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_return(wnoutrefresh(win));
}


void
cmd_wprintw(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_return(wprintw(win, args[1], args[2]));
}


void
cmd_wredrawln(int nargs, char **args)
{
    WINDOW *win;
    int beg_line, num_lines;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&beg_line, args[1]);
    set_int(&num_lines, args[2]);

    report_count(1);
    report_return(wredrawln(win, beg_line, num_lines));
}


void
cmd_wrefresh(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    /* XXX - generates output */
    report_count(1);
    report_return(wrefresh(win));
}


void
cmd_wresize(int nargs, char **args)
{
    WINDOW *win;
    int lines, cols;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&lines, args[1]);
    set_int(&cols, args[2]);

    report_count(1);
    report_return(wresize(win, lines, cols));
}


void
cmd_wscanw(int nargs, char **args)
{
    WINDOW *win;
    char string[256];

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_return(wscanw(win, args[1], &string));
}


void
cmd_wscrl(int nargs, char **args)
{
    WINDOW *win;
    int n;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&n, args[1]);

    report_count(1);
    report_return(wscrl(win, n));
}


void
cmd_wsetscrreg(int nargs, char **args)
{
    WINDOW *win;
    int top, bottom;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&top, args[1]);
    set_int(&bottom, args[2]);

    report_count(1);
    report_return(wsetscrreg(win, top, bottom));
}


void
cmd_wstandend(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_int(wstandend(win));
}


void
cmd_wstandout(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_int(wstandout(win));
}


void
cmd_wtimeout(int nargs, char **args)
{
    WINDOW *win;
    int tval;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&tval, args[1]);

    wtimeout(win, tval); /* void return */
    report_count(1);
    report_return(OK);
}


void
cmd_wtouchln(int nargs, char **args)
{
    WINDOW *win;
    int line, n, changed;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&line, args[1]);
    set_int(&n, args[2]);
    set_int(&changed, args[3]);

    report_count(1);
    report_return(wtouchln(win, line, n, changed));
}


void
cmd_wunderend(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_int(wunderend(win));
}


void
cmd_wunderscore(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_int(wunderscore(win));
}


void
cmd_wvline(int nargs, char **args)
{
    WINDOW *win;
    int n;
    chtype *ch;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    ch = (chtype *) args[1];
    set_int(&n, args[2]);

    report_count(1);
    report_return(wvline(win, ch[0], n));
}


void
cmd_insnstr(int nargs, char **args)
{
    int n;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_int(&n, args[1]);

    report_count(1);
    report_return(insnstr(args[0], n));
}


void
cmd_insstr(int nargs, char **args)
{
    if (check_arg_count(nargs, 1) == 1)
        return;

    report_count(1);
    report_return(insstr(args[0]));
}


void
cmd_mvinsnstr(int nargs, char **args)
{
    int y, x, n;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);
    set_int(&n, args[3]);

    report_count(1);
    report_return(mvinsnstr(y, x, args[2], n));
}


void
cmd_mvinsstr(int nargs, char **args)
{
    int y, x;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);

    report_count(1);
    report_return(mvinsstr(y, x, args[2]));
}


void
cmd_mvwinsnstr(int nargs, char **args)
{
    WINDOW *win;
    int y, x, n;

    if (check_arg_count(nargs, 5) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);
    set_int(&n, args[4]);

    report_count(1);
    report_return(mvwinsnstr(win, y, x, args[3], n));

}


void
cmd_mvwinsstr(int nargs, char **args)
{
    WINDOW *win;
    int y, x;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);

    report_count(1);
    report_return(mvwinsstr(win, y, x, args[3]));
}


void
cmd_winsnstr(int nargs, char **args)
{
    WINDOW *win;
    int n;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&n, args[2]);

    report_count(1);
    report_return(winsnstr(win, args[1], n));
}


void
cmd_winsstr(int nargs, char **args)
{
    WINDOW *win;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    report_return(winsstr(win, args[1]));
}



void
cmd_chgat(int nargs, char **args)
{
    int n, attr, colour;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_int(&n, args[0]);
    set_int(&attr, args[1]);
    set_int(&colour, args[2]);

    /* Note: 4th argument unused in current curses implementation */
    report_count(1);
    report_return(chgat(n, attr, colour, NULL));
}


void
cmd_wchgat(int nargs, char **args)
{
    WINDOW *win;
    int n, attr;
    short colour;

    if (check_arg_count(nargs, 5) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&n, args[1]);
    set_int(&attr, args[2]);
    set_short(&colour, args[3]);

    report_count(1);
    report_return(wchgat(win, n, attr, colour, NULL));
}


void
cmd_mvchgat(int nargs, char **args)
{
    int y, x, n, attr;
    short colour;

    if (check_arg_count(nargs, 6) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);
    set_int(&n, args[2]);
    set_int(&attr, args[3]);
    set_short(&colour, args[4]);

    report_count(1);
    report_return(mvchgat(y, x, n, attr, colour, NULL));
}


void
cmd_mvwchgat(int nargs, char **args)
{
    WINDOW *win;
    int y, x, n, attr;
    short colour;

    if (check_arg_count(nargs, 7) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);
    set_int(&n, args[3]);
    set_int(&attr, args[4]);
    set_short(&colour, args[5]);

    report_count(1);
    report_return(mvwchgat(win, y, x, n, attr, colour, NULL));
}


void
cmd_add_wch(int nargs, char **args)
{
    cchar_t *ch;

    if (check_arg_count(nargs, 1) == 1)
        return;

    ch = (cchar_t *) args[0];

    report_count(1);
    report_return(add_wch(ch));
}


void
cmd_wadd_wch(int nargs, char **args)
{
    WINDOW *win;
    cchar_t *ch;
    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    ch = (cchar_t *) args[1];

    report_count(1);
    report_return(wadd_wch(win, ch));
}


void
cmd_mvadd_wch(int nargs, char **args)
{
    int y, x;
    cchar_t *ch;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);
    ch = (cchar_t *) args[2];

    report_count(1);
    report_return(mvadd_wch(y, x, ch));
}


void
cmd_mvwadd_wch(int nargs, char **args)
{
    WINDOW *win;
    int y, x;
    cchar_t *ch;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);
    ch = (cchar_t *) args[3];

    report_count(1);
    report_return(mvwadd_wch(win, y, x, ch));
}



void
cmd_add_wchnstr(int nargs, char **args)
{
    if (check_arg_count(nargs, 1) == 1)
        return;

    report_count(1);
    report_error("UNSUPPORTED");
}


void
cmd_add_wchstr(int nargs, char **args)
{
    if (check_arg_count(nargs, 1) == 1)
        return;

    report_count(1);
    report_error("UNSUPPORTED");
}


void
cmd_wadd_wchnstr(int nargs, char **args)
{
    if (check_arg_count(nargs, 1) == 1)
        return;

    report_count(1);
    report_error("UNSUPPORTED");
}


void
cmd_wadd_wchstr(int nargs, char **args)
{
    if (check_arg_count(nargs, 1) == 1)
        return;

    report_count(1);
    report_error("UNSUPPORTED");
}


void
cmd_mvadd_wchnstr(int nargs, char **args)
{
    if (check_arg_count(nargs, 1) == 1)
        return;

    report_count(1);
    report_error("UNSUPPORTED");
}


void
cmd_mvadd_wchstr(int nargs, char **args)
{
    if (check_arg_count(nargs, 1) == 1)
        return;

    report_count(1);
    report_error("UNSUPPORTED");
}


void
cmd_mvwadd_wchnstr(int nargs, char **args)
{
    if (check_arg_count(nargs, 1) == 1)
        return;

    report_count(1);
    report_error("UNSUPPORTED");
}


void
cmd_mvwadd_wchstr(int nargs, char **args)
{
    if (check_arg_count(nargs, 1) == 1)
        return;

    report_count(1);
    report_error("UNSUPPORTED");
}



void
cmd_addnwstr(int nargs, char **args)
{
    int n;
    wchar_t *wstr;

    if (check_arg_count(nargs, 2) == 1)
        return;

    wstr = (wchar_t *) args[0];
    set_int(&n, args[1]);

    report_count(1);
    report_return(addnwstr(wstr, n));
}


void
cmd_addwstr(int nargs, char **args)
{
    wchar_t *wstr;

    if (check_arg_count(nargs, 1) == 1)
        return;

    wstr = (wchar_t *) args[0];

    report_count(1);
    report_return(addwstr(wstr));
}


void
cmd_mvaddnwstr(int nargs, char **args)
{
    int y,x,n;
    wchar_t *wstr;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);
    wstr = (wchar_t *) args[2];
    set_int(&n, args[3]);

    report_count(1);
    report_return(mvaddnwstr(y, x, wstr, n));
}


void
cmd_mvaddwstr(int nargs, char **args)
{
    int y,x;
    wchar_t *wstr;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);
    wstr = (wchar_t *) args[2];

    report_count(1);
    report_return(mvaddwstr(y, x, wstr));
}


void
cmd_mvwaddnwstr(int nargs, char **args)
{
    WINDOW *win;
    int y,x,n;
    wchar_t *wstr;

    if (check_arg_count(nargs, 5) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);
    wstr = (wchar_t *) args[3];
    set_int(&n, args[4]);

    report_count(1);
    report_return(mvwaddnwstr(win, y, x, wstr, n));
}


void
cmd_mvwaddwstr(int nargs, char **args)
{
    WINDOW *win;
    int y,x;
    wchar_t *wstr;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);
    wstr = (wchar_t *) args[3];

    report_count(1);
    report_return(mvwaddwstr(win, y, x, wstr));
}


void
cmd_waddnwstr(int nargs, char **args)
{
    WINDOW *win;
    int n;
    wchar_t *wstr;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    wstr = (wchar_t *) args[1];
    set_int(&n, args[2]);

    report_count(1);
    report_return(waddnwstr(win, wstr, n));
}


void
cmd_waddwstr(int nargs, char **args)
{
    WINDOW *win;
    wchar_t *wstr;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    wstr = (wchar_t *) args[1];

    report_count(1);
    report_return(waddwstr(win, wstr));
}



void
cmd_echo_wchar(int nargs, char **args)
{
    cchar_t *ch;

    if (check_arg_count(nargs, 1) == 1)
        return;

    ch = (cchar_t *) args[0];

    report_count(1);
    report_return(echo_wchar(ch));
}


void
cmd_wecho_wchar(int nargs, char **args)
{
    WINDOW *win;
    cchar_t *ch;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    ch = (cchar_t *) args[1];

    report_count(1);
    report_return(wecho_wchar(win, ch));
}


void
cmd_pecho_wchar(int nargs, char **args)
{
    WINDOW *pad;
    cchar_t *wch;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&pad, args[0]);
    wch = (cchar_t *) args[1];

    report_count(1);
    report_return(pecho_wchar(pad, wch));
}



/* insert */
void
cmd_ins_wch(int nargs, char **args)
{
    cchar_t *wch;

    if (check_arg_count(nargs, 1) == 1)
        return;

    wch = (cchar_t *) args[0];

    report_count(1);
    report_return(ins_wch(wch));
}


void
cmd_wins_wch(int nargs, char **args)
{
    WINDOW *win;
    cchar_t *wch;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    wch = (cchar_t *) args[1];
    
    report_count(1);
    report_return(wins_wch(win, wch));
}


void
cmd_mvins_wch(int nargs, char **args)
{
    int y, x;
    cchar_t *wch;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);
    wch = (cchar_t *) args[2];
    
    report_count(1);
    report_return(mvins_wch(y, x, wch));
}


void
cmd_mvwins_wch(int nargs, char **args)
{
    WINDOW *win;
    int y, x;
    cchar_t *wch;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);
    wch = (cchar_t *) args[3];
    
    report_count(1);
    report_return(mvwins_wch(win, y, x, wch));
}



void
cmd_ins_nwstr(int nargs, char **args)
{
    wchar_t *wstr;
    int n;

    if (check_arg_count(nargs, 2) == 1)
        return;

    wstr = (wchar_t *) args[0];
    set_int(&n, args[1]);

    report_count(1);
    report_return(ins_nwstr(wstr, n));
}


void
cmd_ins_wstr(int nargs, char **args)
{
    wchar_t *wstr;

    if (check_arg_count(nargs, 1) == 1)
        return;

    wstr = (wchar_t *) args[0];

    report_count(1);
    report_return(ins_wstr(wstr));
}


void
cmd_mvins_nwstr(int nargs, char **args)
{
    int y, x;
    wchar_t *wstr;
    int n;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);
    wstr = (wchar_t *) args[2];
    set_int(&n, args[3]);

    report_count(1);
    report_return(mvins_nwstr(y, x, wstr, n));
}


void
cmd_mvins_wstr(int nargs, char **args)
{
    int y, x;
    wchar_t *wstr;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);
    wstr = (wchar_t *) args[2];

    report_count(1);
    report_return(mvins_wstr(y, x, wstr));
}


void
cmd_mvwins_nwstr(int nargs, char **args)
{
    WINDOW *win;
    int y, x;
    wchar_t *wstr;
    int n;

    if (check_arg_count(nargs, 5) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);
    wstr = (wchar_t *) args[3];
    set_int(&n, args[4]);

    report_count(1);
    report_return(mvwins_nwstr(win, y, x, wstr, n));
}


void
cmd_mvwins_wstr(int nargs, char **args)
{
    WINDOW *win;
    int y, x;
    wchar_t *wstr;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);
    wstr = (wchar_t *) args[3];

    report_count(1);
    report_return(mvwins_wstr(win, y, x, wstr));
}


void
cmd_wins_nwstr(int nargs, char **args)
{
    WINDOW *win;
    wchar_t *wstr;
    int n;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    wstr = (wchar_t *) args[1];
    set_int(&n, args[2]);

    report_count(1);
    report_return(wins_nwstr(win, wstr, n));
}


void
cmd_wins_wstr(int nargs, char **args)
{
    WINDOW *win;
    wchar_t *wstr;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    wstr = (wchar_t *) args[1];

    report_count(1);
    report_return(wins_wstr(win, wstr));
}



/* input */
void
cmd_get_wch(int nargs, char **args)
{
    wchar_t ch;
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(2);
    report_return(get_wch(&ch));
    report_wchar(ch);
}


void
cmd_unget_wch(int nargs, char **args)
{
    wchar_t *wch;
    if (check_arg_count(nargs, 1) == 1)
        return;

    wch = (wchar_t *) args[0];

    report_count(1);
    report_return(unget_wch(*wch));
}


void
cmd_mvget_wch(int nargs, char **args)
{
    wchar_t ch;
    int y, x;
    if (check_arg_count(nargs, 2) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);

    report_count(2);
    report_return(mvget_wch(y, x, &ch));
    report_wchar(ch);
}


void
cmd_mvwget_wch(int nargs, char **args)
{
    wchar_t ch;
    WINDOW *win;
    int y, x;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);

    report_count(2);
    report_return(mvwget_wch(win, y, x, &ch));
    report_wchar(ch);
}


void
cmd_wget_wch(int nargs, char **args)
{
    wchar_t ch;
    WINDOW *win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(2);
    report_return(wget_wch(win, &ch));
    report_wchar(ch);
}



void
cmd_getn_wstr(int nargs, char **args)
{
    wchar_t wstr[256];
    int n;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_int(&n, args[0]);

    report_count(2);
    report_return(getn_wstr(wstr, n));
    report_wstr(wstr);
}


void
cmd_get_wstr(int nargs, char **args)
{
    wchar_t wstr[256];

    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(2);
    report_return(get_wstr(wstr));
    report_wstr(wstr);
}

void
cmd_mvgetn_wstr(int nargs, char **args)
{
    wchar_t wstr[256];
    int y, x, n;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);
    set_int(&n, args[2]);

    report_count(2);
    report_return(mvgetn_wstr(y, x, wstr, n));
    report_wstr(wstr);
}

void
cmd_mvget_wstr(int nargs, char **args)
{
    wchar_t wstr[256];
    int y, x;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);

    report_count(2);
    report_return(mvget_wstr(y, x, wstr));
    report_wstr(wstr);
}


void
cmd_mvwgetn_wstr(int nargs, char **args)
{
    WINDOW *win;
    wchar_t wstr[256];
    int y, x, n;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);
    set_int(&n, args[3]);

    report_count(2);
    report_return(mvwgetn_wstr(win, y, x, wstr, n));
    report_wstr(wstr);
}


void
cmd_mvwget_wstr(int nargs, char **args)
{
    WINDOW *win;
    wchar_t wstr[256];
    int y, x;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);

    report_count(2);
    report_return(mvwget_wstr(win, y, x, wstr));
    report_wstr(wstr);
}


void
cmd_wgetn_wstr(int nargs, char **args)
{
    WINDOW *win;
    wchar_t wstr[256];
    int n;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&n, args[1]);

    report_count(2);
    report_return(wgetn_wstr(win, wstr, n));
    report_wstr(wstr);
}


void
cmd_wget_wstr(int nargs, char **args)
{
    WINDOW *win;
    wchar_t wstr[256];

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(2);
    report_return(wget_wstr(win, wstr));
    report_wstr(wstr);
}



void
cmd_in_wch(int nargs, char **args)
{
    cchar_t wcval;
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(2);
    report_return(in_wch(&wcval));
    report_cchar(wcval);
}


void
cmd_mvin_wch(int nargs, char **args)
{
    cchar_t wcval;
    int y,x;
    if (check_arg_count(nargs, 2) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);

    report_count(2);
    report_return(mvin_wch(y, x, &wcval));
    report_cchar(wcval);
}


void
cmd_mvwin_wch(int nargs, char **args)
{
    cchar_t wcval;
    WINDOW *win;
    int y,x;
    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);

    report_count(2);
    report_return(mvwin_wch(win, y, x, &wcval));
    report_cchar(wcval);
}


void
cmd_win_wch(int nargs, char **args)
{
    cchar_t wcval;
    WINDOW *win;
    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(2);
    report_return(win_wch(win, &wcval));
    report_cchar(wcval);
}


void
cmd_in_wchnstr(int nargs, char **args)
{
    if (check_arg_count(nargs, 1) == 1)
        return;

    report_count(1);
    report_error("UNSUPPORTED");
}


void
cmd_in_wchstr(int nargs, char **args)
{
    if (check_arg_count(nargs, 1) == 1)
        return;

    report_count(1);
    report_error("UNSUPPORTED");
}


void
cmd_mvin_wchnstr(int nargs, char **args)
{
    if (check_arg_count(nargs, 1) == 1)
        return;

    report_count(1);
    report_error("UNSUPPORTED");
}


void
cmd_mvin_wchstr(int nargs, char **args)
{
    if (check_arg_count(nargs, 1) == 1)
        return;

    report_count(1);
    report_error("UNSUPPORTED");
}


void
cmd_mvwin_wchnstr(int nargs, char **args)
{
    if (check_arg_count(nargs, 1) == 1)
        return;

    report_count(1);
    report_error("UNSUPPORTED");
}


void
cmd_mvwin_wchstr(int nargs, char **args)
{
    if (check_arg_count(nargs, 1) == 1)
        return;

    report_count(1);
    report_error("UNSUPPORTED");
}


void
cmd_win_wchnstr(int nargs, char **args)
{
    if (check_arg_count(nargs, 1) == 1)
        return;

    report_count(1);
    report_error("UNSUPPORTED");
}


void
cmd_win_wchstr(int nargs, char **args)
{
    if (check_arg_count(nargs, 1) == 1)
        return;

    report_count(1);
    report_error("UNSUPPORTED");
}



void
cmd_innwstr(int nargs, char **args)
{
    wchar_t wstr[256];
    int n;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_int(&n, args[0]);

    report_count(2);
    report_int(innwstr(wstr, n));
    report_wstr(wstr);
}


void
cmd_inwstr(int nargs, char **args)
{
    wchar_t wstr[256];
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(2);
    report_return(inwstr(wstr));
    report_wstr(wstr);
}


void
cmd_mvinnwstr(int nargs, char **args)
{
    wchar_t wstr[256];
    int y, x, n;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);
    set_int(&n, args[2]);

    report_count(2);
    report_int(mvinnwstr(y, x, wstr, n));
    report_wstr(wstr);
}


void
cmd_mvinwstr(int nargs, char **args)
{
    wchar_t wstr[256];
    int y, x;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);

    report_count(2);
    report_return(mvinwstr(y, x, wstr));
    report_wstr(wstr);
}


void
cmd_mvwinnwstr(int nargs, char **args)
{
    WINDOW *win;
    wchar_t wstr[256];
    int y, x, n;

    if (check_arg_count(nargs, 4) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);
    set_int(&n, args[3]);

    report_count(2);
    report_int(mvwinnwstr(win, y, x, wstr, n));
    report_wstr(wstr);
}


void
cmd_mvwinwstr(int nargs, char **args)
{
    WINDOW *win;
    wchar_t wstr[256];
    int y, x;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);

    report_count(2);
    report_return(mvwinwstr(win, y, x, wstr));
    report_wstr(wstr);
}


void
cmd_winnwstr(int nargs, char **args)
{
    WINDOW *win;
    wchar_t wstr[256];
    int n;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&n, args[1]);

    report_count(2);
    report_int(winnwstr(win, wstr, n));
    report_wstr(wstr);
}


void
cmd_winwstr(int nargs, char **args)
{
    WINDOW *win;
    wchar_t wstr[256];

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(2);
    report_return(winwstr(win, wstr));
    report_wstr(wstr);
}



/* cchar handling */
void
cmd_setcchar(int nargs, char **args)
{
    cchar_t wcval;
    wchar_t *wch;
    attr_t attrs;
    short color_pair;

    if (check_arg_count(nargs, 4) == 1)
        return;

    wch = (wchar_t *) args[0];
    set_int(&attrs, args[1]);
    set_short(&color_pair, args[2]);

    report_count(2);
    report_return(setcchar(&wcval, wch, attrs, color_pair, NULL));
    report_cchar(wcval);
}


void
cmd_getcchar(int nargs, char **args)
{
    cchar_t *wcval;
    wchar_t wch[256];
    attr_t attrs;
    short color_pair;

    /*
     * XXX - not handling passing of wch as NULL
     */

    if (check_arg_count(nargs, 2) == 1)
        return;

    wcval = (cchar_t *) args[0];

    report_count(4);
    report_return(getcchar(wcval, wch, &attrs, &color_pair, NULL));
    report_wstr(wch);
    report_int(attrs);
    report_int(color_pair);
}



/* misc */
void
cmd_key_name(int nargs, char **args)
{
    wchar_t w;

    if (check_arg_count(nargs, 1) == 1)
        return;

    w = *((wchar_t *) args[0]);

    report_count(1);
    report_status(key_name(w));
}


void
cmd_border_set(int nargs, char **args)
{
    cchar_t *ls, *rs, *ts, *bs, *tl, *tr, *bl, *br;

    if (check_arg_count(nargs, 8) == 1)
        return;

    ls = (cchar_t *) args[0];
    rs = (cchar_t *) args[1];
    ts = (cchar_t *) args[2];
    bs = (cchar_t *) args[3];
    tl = (cchar_t *) args[4];
    tr = (cchar_t *) args[5];
    bl = (cchar_t *) args[6];
    br = (cchar_t *) args[7];

    report_count(1);
    report_return(border_set(ls, rs, ts, bs, tl, tr, bl, br));
}


void
cmd_wborder_set(int nargs, char **args)
{
    WINDOW *win;
    cchar_t *ls, *rs, *ts, *bs, *tl, *tr, *bl, *br;

    if (check_arg_count(nargs, 9) == 1)
        return;

    set_win(&win, args[0]);
    ls = (cchar_t *) args[1];
    rs = (cchar_t *) args[2];
    ts = (cchar_t *) args[3];
    bs = (cchar_t *) args[4];
    tl = (cchar_t *) args[5];
    tr = (cchar_t *) args[6];
    bl = (cchar_t *) args[7];
    br = (cchar_t *) args[8];

    report_count(1);
    report_return(wborder_set(win, ls, rs, ts, bs, tl, tr, bl, br));
}


void
cmd_box_set(int nargs, char **args)
{
    WINDOW *win;
    cchar_t *verch, *horch;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    verch = (cchar_t *) args[1];
    horch = (cchar_t *) args[2];

    report_count(1);
    report_return(box_set(win, verch, horch));
}


void
cmd_erasewchar(int nargs, char **args)
{
    wchar_t ch;

    if (check_arg_count(nargs, 0) == 1)
        return;

    /* XXX - call2 */
    report_count(2);
    report_return(erasewchar(&ch));
    report_wchar(ch);
}


void
cmd_killwchar(int nargs, char **args)
{
    wchar_t ch;

    if (check_arg_count(nargs, 0) == 1)
        return;

    /* XXX - call2 */
    report_count(2);
    report_return(killwchar(&ch));
    report_wchar(ch);
}


void
cmd_hline_set(int nargs, char **args)
{
    cchar_t *wch;
    int n;

    if (check_arg_count(nargs, 2) == 1)
        return;

    wch = (cchar_t *) args[0];
    set_int(&n, args[1]);

    report_count(1);
    report_return(hline_set(wch, n));
}


void
cmd_mvhline_set(int nargs, char **args)
{
    cchar_t *wch;
    int y, x, n;
    if (check_arg_count(nargs, 4) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);
    wch = (cchar_t *) args[2];
    set_int(&n, args[3]);

    report_count(1);
    report_return(mvhline_set(y, x, wch, n));
}


void
cmd_mvvline_set(int nargs, char **args)
{
    cchar_t *wch;
    int y, x, n;
    if (check_arg_count(nargs, 4) == 1)
        return;

    set_int(&y, args[0]);
    set_int(&x, args[1]);
    wch = (cchar_t *) args[2];
    set_int(&n, args[3]);

    report_count(1);
    report_return(mvvline_set(y, x, wch, n));
}


void
cmd_mvwhline_set(int nargs, char **args)
{
    WINDOW *win;
    cchar_t *wch;
    int y, x, n;
    if (check_arg_count(nargs, 5) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);
    wch = (cchar_t *) args[3];
    set_int(&n, args[4]);

    report_count(1);
    report_return(mvwhline_set(win, y, x, wch, n));
}


void
cmd_mvwvline_set(int nargs, char **args)
{
    WINDOW *win;
    cchar_t *wch;
    int y, x, n;
    if (check_arg_count(nargs, 5) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&y, args[1]);
    set_int(&x, args[2]);
    wch = (cchar_t *) args[3];
    set_int(&n, args[4]);

    report_count(1);
    report_return(mvwvline_set(win, y, x, wch, n));
}


void
cmd_vline_set(int nargs, char **args)
{
    cchar_t *wch;
    int n;

    if (check_arg_count(nargs, 2) == 1)
        return;

    wch = (cchar_t *) args[0];
    set_int(&n, args[1]);

    report_count(1);
    report_return(vline_set(wch, n));
}


void
cmd_whline_set(int nargs, char **args)
{
    WINDOW *win;
    cchar_t *wch;
    int n;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    wch = (cchar_t *) args[1];
    set_int(&n, args[2]);

    report_count(1);
    report_return(whline_set(win, wch, n));
}


void
cmd_wvline_set(int nargs, char **args)
{
    WINDOW *win;
    cchar_t *wch;
    int n;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_win(&win, args[0]);
    wch = (cchar_t *) args[1];
    set_int(&n, args[2]);

    report_count(1);
    report_return(wvline_set(win, wch, n));
}


void
cmd_bkgrnd(int nargs, char **args)
{
    cchar_t *wch;
    if (check_arg_count(nargs, 1) == 1)
        return;

    wch = (cchar_t *) args[0];

    report_count(1);
    report_return(bkgrnd(wch));
}


void
cmd_bkgrndset(int nargs, char **args)
{
    cchar_t *wch;
    if (check_arg_count(nargs, 1) == 1)
        return;

    wch = (cchar_t *) args[0];

    report_count(1);
    bkgrndset(wch);
    report_return(OK);
}


void
cmd_getbkgrnd(int nargs, char **args)
{
    cchar_t wch;
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(2);
    report_return(getbkgrnd(&wch));
    report_cchar(wch);
}


void
cmd_wbkgrnd(int nargs, char **args)
{
    WINDOW *win;
    cchar_t *wch;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    wch = (cchar_t *) args[1];

    report_count(1);
    report_return(wbkgrnd(win, wch));
}


void
cmd_wbkgrndset(int nargs, char **args)
{
    WINDOW *win;
    cchar_t *wch;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    wch = (cchar_t *) args[1];

    report_count(1);
    wbkgrndset(win, wch);
    report_return(OK);
}


void
cmd_wgetbkgrnd(int nargs, char **args)
{
    WINDOW *win;
    cchar_t wch;
    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(2);
    report_return(wgetbkgrnd(win, &wch));
    report_cchar(wch);
}


void
cmd_immedok(int nargs, char **args)
{
    WINDOW* win;
    int bf;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&bf, args[1]);

    report_count(1);
    immedok(win, bf);
    report_return(OK);
}

void
cmd_syncok(int nargs, char **args)
{
    WINDOW* win;
    int bf;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_win(&win, args[0]);
    set_int(&bf, args[1]);

    report_count(1);
    report_return(syncok(win, bf));
}

void
cmd_wcursyncup(int nargs, char **args)
{
    WINDOW* win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    wcursyncup(win);
    report_return(OK);
}

void
cmd_wsyncup(int nargs, char **args)
{
    WINDOW* win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    wsyncup(win);
    report_return(OK);
}

void
cmd_wsyncdown(int nargs, char **args)
{
    WINDOW* win;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_win(&win, args[0]);

    report_count(1);
    wsyncdown(win);
    report_return(OK);
}


/* Soft label key routines */
void
cmd_slk_attroff(int nargs, char **args)
{
    chtype *ch;

    if (check_arg_count(nargs, 1) == 1)
        return;

    ch = (chtype *) args[0];

    report_count(1);
    report_return(slk_attroff(ch[0]));
}

void
cmd_slk_attr_off(int nargs, char **args)
{
    attr_t attrs;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_int(&attrs, args[0]);

    report_count(1);
    report_return(slk_attr_off(attrs, NULL));
}

void
cmd_slk_attron(int nargs, char **args)
{
    chtype *ch;

    if (check_arg_count(nargs, 1) == 1)
        return;

    ch = (chtype *) args[0];

    report_count(1);
    report_return(slk_attron(ch[0]));
}

void
cmd_slk_attr_on(int nargs, char **args)
{
    chtype attrs;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_int(&attrs, args[0]);

    report_count(1);
    report_return(slk_attr_on(attrs, NULL));
}

void
cmd_slk_attrset(int nargs, char **args)
{
    chtype *ch;

    if (check_arg_count(nargs, 1) == 1)
        return;

    ch = (chtype *) args[0];

    report_count(1);
    report_return(slk_attrset(ch[0]));
}

void
cmd_slk_attr_set(int nargs, char **args)
{
    attr_t attrs;
    short color_pair_number;

    if (check_arg_count(nargs, 2) == 1)
        return;

    set_int(&attrs, args[0]);
    set_short(&color_pair_number, args[1]);

    report_count(1);
    report_return(slk_attr_set(attrs, color_pair_number, NULL));
}

void
cmd_slk_clear(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(slk_clear());
}

void
cmd_slk_color(int nargs, char **args)
{
    short color_pair_number;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_short(&color_pair_number, args[0]);

    report_count(1);
    report_return(slk_color(color_pair_number));
}

void
cmd_slk_label(int nargs, char **args)
{
    char *label;
    int labnum;

    if (check_arg_count(nargs, 1) == 1)
        return;

    set_int(&labnum, args[0]);

    label = slk_label(labnum);
    report_count(1);
    if(label == NULL)
        report_status("NULL");
    else
        report_status(label);
}

void
cmd_slk_noutrefresh(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(slk_noutrefresh());
}

void
cmd_slk_refresh(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(slk_refresh());
}

void
cmd_slk_restore(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(slk_restore());
}

void
cmd_slk_set(int nargs, char **args)
{
    int labnum;
    int justify;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_int(&labnum, args[0]);
    set_int(&justify, args[2]);

    report_count(1);
    report_return(slk_set(labnum, args[1], justify));
}

void
cmd_slk_touch(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    report_return(slk_touch());
}

void
cmd_slk_wset(int nargs, char **args)
{
    int labnum;
    wchar_t *label;
    int justify;

    if (check_arg_count(nargs, 3) == 1)
        return;

    set_int(&labnum, args[0]);
    label = (wchar_t *) args[1];
    set_int(&justify, args[2]);

    report_count(1);
    report_return(slk_wset(labnum, label, justify));
}


void
cmd_slk_init(int nargs, char **args)
{
    int fmt;
    if (check_arg_count(nargs, 1) == 1)
        return;

    set_int(&fmt, args[0]);

    report_count(1);
    report_return(slk_init(fmt));
}

void
cmd_use_env(int nargs, char **args)
{
    if (check_arg_count(nargs, 1) == 1)
        return;

    report_count(1);
    report_error("UNSUPPORTED");
}

void
cmd_ripoffline(int nargs, char **args)
{
    if (check_arg_count(nargs, 1) == 1)
        return;

    report_count(1);
    report_error("UNSUPPORTED");
}

void
cmd_filter(int nargs, char **args)
{
    if (check_arg_count(nargs, 0) == 1)
        return;

    report_count(1);
    filter();
    report_return(OK);
}