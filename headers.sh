##########################################
# curses add characters to window routines
##########################################

atf_test_case addch
addch_head()
{
	atf_set "descr" "Tests adding a chtype to stdscr"
}
addch_body()
{
	h_run addch
}

atf_test_case waddch
waddch_head()
{
    atf_set "descr" ""
}
waddch_body()
{
    h_run waddch "Tests adding a chtype to window - tests mvwaddch too"
}

atf_test_case mvaddch
mvaddch_head()
{
	atf_set "descr" "Move the cursor and add a character to stdscr"
}
mvaddch_body()
{
	h_run mvaddch
}

atf_test_case addchstr
addchstr_head()
{
	atf_set "descr" "Tests adding a chtype string to stdscr"
}
addchstr_body()
{
	h_run addchstr
}

atf_test_case waddchstr
waddchstr_head()
{
    atf_set "descr" ""
}
waddchstr_body()
{
    h_run waddchstr
}

atf_test_case addchnstr
addchnstr_head()
{
	atf_set "descr" "Tests adding bytes from a chtype string to stdscr"
}
addchnstr_body()
{
	h_run addchnstr
}

atf_test_case waddchnstr
waddchnstr_head()
{
    atf_set "descr" ""
}
waddchnstr_body()
{
    h_run waddchnstr
}

atf_test_case mvaddchstr
mvaddchstr_head()
{
	atf_set "descr" "Move the cursor and add a ch string to stdscr"
}
mvaddchstr_body()
{
	h_run mvaddchstr
}

atf_test_case mvwaddchstr
mvwaddchstr_head()
{
    atf_set "descr" ""
}
mvwaddchstr_body()
{
    h_run mvwaddchstr
}

atf_test_case mvaddchnstr
mvaddchnstr_head()
{
	atf_set "descr" "Move the cursor and add a character string to stdscr"
}
mvaddchnstr_body()
{
	h_run mvaddchnstr
}

atf_test_case mvwaddchnstr
mvwaddchnstr_head()
{
    atf_set "descr" ""
}
mvwaddchnstr_body()
{
    h_run mvwaddchnstr
}

atf_test_case addstr
addstr_head()
{
	atf_set "descr" "Tests adding bytes from a string to stdscr"
}
addstr_body()
{
	h_run addstr
}

atf_test_case addwstr
addwstr_head()
{
    atf_set "descr" ""
}
addwstr_body()
{
    h_run addwstr
}

atf_test_case waddstr
waddstr_head()
{
    atf_set "descr" ""
}
waddstr_body()
{
    h_run waddstr
}

atf_test_case waddwstr
waddwstr_head()
{
    atf_set "descr" ""
}
waddwstr_body()
{
    h_run waddwstr
}

atf_test_case addnstr
addnstr_head()
{
	atf_set "descr" "Tests adding bytes from a string to stdscr"
}
addnstr_body()
{
	h_run addnstr
}

atf_test_case addnwstr
addnwstr_head()
{
    atf_set "descr" ""
}
addnwstr_body()
{
    h_run addnwstr
}

atf_test_case waddnstr
waddnstr_head()
{
    atf_set "descr" ""
}
waddnstr_body()
{
    h_run waddnstr
}

atf_test_case mvwaddnwstr
mvwaddnwstr_head()
{
    atf_set "descr" ""
}
mvwaddnwstr_body()
{
    h_run mvwaddnwstr
}

atf_test_case mvaddstr
mvaddstr_head()
{
	atf_set "descr" "Move the cursor and add a string to stdscr"
}
mvaddstr_body()
{
	h_run mvaddstr
}

atf_test_case mvaddwstr
mvaddwstr_head()
{
    atf_set "descr" ""
}
mvaddwstr_body()
{
    h_run mvaddwstr
}

atf_test_case mvwaddwstr
mvwaddwstr_head()
{
    atf_set "descr" ""
}
mvwaddwstr_body()
{
    h_run mvwaddwstr
}

atf_test_case mvwaddstr
mvwaddstr_head()
{
    atf_set "descr" ""
}
mvwaddstr_body()
{
    h_run mvwaddstr
}

atf_test_case mvwaddwstr
mvwaddwstr_head()
{
    atf_set "descr" ""
}
mvwaddwstr_body()
{
    h_run mvwaddwstr
}

atf_test_case mvaddnstr
mvaddnstr_head()
{
	atf_set "descr" "Move the cursor and add a limited string to stdscr"
}
mvaddnstr_body()
{
	h_run mvaddnstr
}

atf_test_case mvaddnwstr
mvaddnwstr_head()
{
    atf_set "descr" ""
}
mvaddnwstr_body()
{
    h_run mvaddnwstr
}

atf_test_case mvwaddnstr
mvwaddnstr_head()
{
    atf_set "descr" ""
}
mvwaddnstr_body()
{
    h_run mvwaddnstr
}

atf_test_case mvaddnwstr
mvaddnwstr_head()
{
    atf_set "descr" ""
}
mvaddnwstr_body()
{
    h_run mvaddnwstr
}

atf_test_case mvwaddnwstr
mvwaddnwstr_head()
{
    atf_set "descr" ""
}
mvwaddnwstr_body()
{
    h_run mvwaddnwstr
}

atf_test_case add_wch
add_wch_head()
{
	atf_set "descr" "Test adding complex character to stdscr"
}
add_wch_body()
{
	h_run add_wch en_US.UTF-8
}

atf_test_case wadd_wch
wadd_wch_head()
{
    atf_set "descr" ""
}
wadd_wch_body()
{
    h_run wadd_wch
}

##########################################
# curses input stream routines
##########################################

atf_test_case getch
getch_head()
{
	atf_set "descr" "Checks reading a character input - tests mvgetch also"
}
getch_body()
{
	h_run getch
}

atf_test_case wgetch
wgetch_head()
{
	atf_set "descr" "Checks reading a character input from window - tests mvwgetch also"
}
wgetch_body()
{
	h_run wgetch
}

atf_test_case define_key
define_key_head()
{
	atf_set "descr" "Check defining a key and removing the definition works"
}
define_key_body()
{
	h_run define_key
}

atf_test_case keyok
keyok_head()
{
	atf_set "descr" "Check the ability to disable interpretation of a multichar key sequence"
}
keyok_body()
{
	h_run keyok
}

atf_test_case getnstr
getnstr_head()
{
	atf_set "descr" "Check getting a string with a limit"
}
getnstr_body()
{
	h_run getnstr
}

atf_test_case wgetnstr
wgetnstr_head()
{
    atf_set "descr" ""
}
wgetnstr_body()
{
    h_run wgetnstr
}

atf_test_case mvgetnstr
mvgetnstr_head()
{
	atf_set "descr" "Move the cursor and get a limited number of characters"
}
mvgetnstr_body()
{
	h_run mvgetnstr
}

atf_test_case mvwgetnstr
mvwgetnstr_head()
{
    atf_set "descr" ""
}
mvwgetnstr_body()
{
    h_run mvwgetnstr
}

atf_test_case getstr
getstr_head()
{
	atf_set "descr" "Check getting a string from input"
}
getstr_body()
{
	h_run getstr
}

atf_test_case wgetstr
wgetstr_head()
{
    atf_set "descr" ""
}
wgetstr_body()
{
    h_run wgetstr
}

atf_test_case mvgetstr
mvgetstr_head()
{
	atf_set "descr" "Move the cursor and get characters"
}
mvgetstr_body()
{
	h_run mvgetstr
}

atf_test_case mvwgetstr
mvwgetstr_head()
{
    atf_set "descr" ""
}
mvwgetstr_body()
{
    h_run mvwgetstr
}

atf_test_case keypad
keypad_head()
{
    atf_set "descr" ""
}
keypad_body()
{
    h_run keypad
}

atf_test_case keypad
keypad_head()
{
    atf_set "descr" "Checks enable/disable abbreviation of function keys - tests is_keypad also"
}
keypad_body()
{
    h_run keypad
}

atf_test_case notimeout
notimeout_head()
{
    atf_set "descr" ""
}
notimeout_body()
{
    h_run notimeout
}

atf_test_case timeout
timeout_head()
{
	atf_set "descr" "Checks timeout when reading a character"
}
timeout_body()
{
	h_run timeout
}

atf_test_case wtimeout
wtimeout_head()
{
    atf_set "descr" ""
}
wtimeout_body()
{
    h_run wtimeout
}

atf_test_case nodelay
nodelay_head()
{
	atf_set "descr" "Test that the nodelay call causes wget to not block"
}
nodelay_body()
{
	h_run nodelay
}

atf_test_case unget_wch
unget_wch_head()
{
    atf_set "descr" "Checks pushing of character into input queue - tests ungetch also"
}
unget_wch_body()
{
    h_run unget_wch
}

atf_test_case getn_wstr
getn_wstr_head()
{
    atf_set "descr" ""
}
getn_wstr_body()
{
    h_run getn_wstr
}

atf_test_case wgetn_wstr
wgetn_wstr_head()
{
    atf_set "descr" ""
}
wgetn_wstr_body()
{
    h_run wgetn_wstr
}

atf_test_case get_wstr
get_wstr_head()
{
    atf_set "descr" ""
}
get_wstr_body()
{
    h_run get_wstr
}

atf_test_case wget_wstr
wget_wstr_head()
{
    atf_set "descr" ""
}
wget_wstr_body()
{
    h_run wget_wstr
}

atf_test_case mvgetn_wstr
mvgetn_wstr_head()
{
    atf_set "descr" ""
}
mvgetn_wstr_body()
{
    h_run mvgetn_wstr
}

atf_test_case mvwgetn_wstr
mvwgetn_wstr_head()
{
    atf_set "descr" ""
}
mvwgetn_wstr_body()
{
    h_run mvwgetn_wstr
}

atf_test_case mvget_wstr
mvget_wstr_head()
{
    atf_set "descr" ""
}
mvget_wstr_body()
{
    h_run mvget_wstr
}

atf_test_case mvwget_wstr
mvwget_wstr_head()
{
    atf_set "descr" ""
}
mvwget_wstr_body()
{
    h_run mvwget_wstr
}

##########################################
# curses read screen contents routines
##########################################

atf_test_case inch
inch_head()
{
	atf_set "descr" "Get the character under the cursor on stdscr"
}
inch_body()
{
	h_run inch
}

atf_test_case winch
winch_head()
{
    atf_set "descr" ""
}
winch_body()
{
    h_run winch
}

atf_test_case inchnstr
inchnstr_head()
{
	atf_set "descr" "Get a limited chtype string from the screen - tests inchstr too"
}
inchnstr_body()
{
	h_run inchnstr
}

atf_test_case winchnstr
winchnstr_head()
{
    atf_set "descr" ""
}
winchnstr_body()
{
    h_run winchnstr
}

atf_test_case mvinchnstr
mvinchnstr_head()
{
	atf_set "descr" "Move the cursor read characters - tests both mvinchstr and mvinchnstr"
}
mvinchnstr_body()
{
	h_run mvinchnstr
}

atf_test_case mvwinchnstr
mvwinchnstr_head()
{
    atf_set "descr" ""
}
mvwinchnstr_body()
{
    h_run mvwinchnstr
}

atf_test_case innstr
innstr_head()
{
	atf_set "descr" "Get a limited string starting at the cursor - tests instr also"
}
innstr_body()
{
	h_run innstr
}

atf_test_case winnstr
winnstr_head()
{
    atf_set "descr" ""
}
winnstr_body()
{
    h_run winnstr
}

atf_test_case mvinnstr
mvinnstr_head()
{
    atf_set "descr" ""
}
mvinnstr_body()
{
    h_run mvinnstr
}

atf_test_case mvwinnstr
mvwinnstr_head()
{
    atf_set "descr" ""
}
mvwinnstr_body()
{
    h_run mvwinnstr
}

atf_test_case in_wch
in_wch_head()
{
    atf_set "descr" " - tests mvin_wch too"
}
in_wch_body()
{
    h_run in_wch
}

atf_test_case win_wch
win_wch_head()
{
    atf_set "descr" " - tests mvwin_wch too"
}
win_wch_body()
{
    h_run win_wch
}

atf_test_case innwstr
innwstr_head()
{
    atf_set "descr" ""
}
innwstr_body()
{
    h_run innwstr
}

atf_test_case winnwstr
winnwstr_head()
{
    atf_set "descr" ""
}
winnwstr_body()
{
    h_run winnwstr
}

atf_test_case inwstr
inwstr_head()
{
    atf_set "descr" ""
}
inwstr_body()
{
    h_run inwstr
}

atf_test_case winwstr
winwstr_head()
{
    atf_set "descr" ""
}
winwstr_body()
{
    h_run winwstr
}

atf_test_case mvinnwstr
mvinnwstr_head()
{
    atf_set "descr" ""
}
mvinnwstr_body()
{
    h_run mvinnwstr
}

atf_test_case mvwinnwstr
mvwinnwstr_head()
{
    atf_set "descr" ""
}
mvwinnwstr_body()
{
    h_run mvwinnwstr
}

atf_test_case mvinwstr
mvinwstr_head()
{
    atf_set "descr" ""
}
mvinwstr_body()
{
    h_run mvinwstr
}

atf_test_case mvwinwstr
mvwinwstr_head()
{
    atf_set "descr" ""
}
mvwinwstr_body()
{
    h_run mvwinwstr
}

##########################################
# curses insert character to window routines
##########################################

atf_test_case insch
insch_head()
{
    atf_set "descr" ""
}
insch_body()
{
    h_run insch
}

atf_test_case winsch
winsch_head()
{
    atf_set "descr" ""
}
winsch_body()
{
    h_run winsch
}

atf_test_case mvinsch
mvinsch_head()
{
    atf_set "descr" ""
}
mvinsch_body()
{
    h_run mvinsch
}

atf_test_case mvwinsch
mvwinsch_head()
{
    atf_set "descr" ""
}
mvwinsch_body()
{
    h_run mvwinsch
}

atf_test_case ins_wch
ins_wch_head()
{
    atf_set "descr" ""
}
ins_wch_body()
{
    h_run ins_wch
}

atf_test_case wins_wch
wins_wch_head()
{
    atf_set "descr" ""
}
wins_wch_body()
{
    h_run wins_wch
}

atf_test_case mvins_wch
mvins_wch_head()
{
    atf_set "descr" ""
}
mvins_wch_body()
{
    h_run mvins_wch
}

atf_test_case mvwins_wch
mvwins_wch_head()
{
    atf_set "descr" ""
}
mvwins_wch_body()
{
    h_run mvwins_wch
}

atf_test_case ins_nwstr
ins_nwstr_head()
{
    atf_set "descr" ""
}
ins_nwstr_body()
{
    h_run ins_nwstr
}

atf_test_case wins_nwstr
wins_nwstr_head()
{
    atf_set "descr" ""
}
wins_nwstr_body()
{
    h_run wins_nwstr
}

atf_test_case ins_wstr
ins_wstr_head()
{
    atf_set "descr" ""
}
ins_wstr_body()
{
    h_run ins_wstr
}

atf_test_case wins_wstr
wins_wstr_head()
{
    atf_set "descr" ""
}
wins_wstr_body()
{
    h_run wins_wstr
}

atf_test_case mvins_nwstr
mvins_nwstr_head()
{
    atf_set "descr" ""
}
mvins_nwstr_body()
{
    h_run mvins_nwstr
}

atf_test_case mvwins_nwstr
mvwins_nwstr_head()
{
    atf_set "descr" ""
}
mvwins_nwstr_body()
{
    h_run mvwins_nwstr
}

atf_test_case mvins_wstr
mvins_wstr_head()
{
    atf_set "descr" ""
}
mvins_wstr_body()
{
    h_run mvins_wstr
}

atf_test_case mvwins_wstr
mvwins_wstr_head()
{
    atf_set "descr" ""
}
mvwins_wstr_body()
{
    h_run mvwins_wstr
}

##########################################
# curses terminal manipulation routines
##########################################

atf_test_case beep
beep_head()
{
	atf_set "descr" "Check sending a beep"
}
beep_body()
{
	h_run beep
}

atf_test_case flash
flash_head()
{
	atf_set "descr" "Validate curses can flash the screen"
}
flash_body()
{
	h_run flash
}

atf_test_case curs_set
curs_set_head()
{
	atf_set "descr" "Check setting the cursor visibility works"
}
curs_set_body()
{
	h_run curs_set
}

atf_test_case delay_output
delay_output_head()
{
    atf_set "descr" ""
}
delay_output_body()
{
    h_run delay_output
}

atf_test_case erasechar
erasechar_head()
{
	atf_set "descr" "Validate erase char can be retrieved"
}
erasechar_body()
{
	h_run erasechar
}

atf_test_case erasewchar
erasewchar_head()
{
    atf_set "descr" ""
}
erasewchar_body()
{
    h_run erasewchar
}

atf_test_case echochar
echochar_head()
{
    atf_set "descr" ""
}
echochar_body()
{
    h_run echochar
}

atf_test_case echo_wchar
echo_wchar_head()
{
    atf_set "descr" ""
}
echo_wchar_body()
{
    h_run echo_wchar
}

atf_test_case halfdelay
halfdelay_head()
{
    atf_set "descr" ""
}
halfdelay_body()
{
    h_run halfdelay
}

atf_test_case has_ic
has_ic_head()
{
	atf_set "descr" "Check if the terminal can insert characters and lines"
}
has_ic_body()
{
	h_run has_ic
}

atf_test_case killchar
killchar_head()
{
	atf_set "descr" "Get the value of the terminals kill character"
}
killchar_body()
{
	h_run killchar
}

atf_test_case killwchar
killwchar_head()
{
    atf_set "descr" ""
}
killwchar_body()
{
    h_run killwchar
}

atf_test_case meta
meta_head()
{
	atf_set "descr" "Check setting and clearing the meta flag on a window"
}
meta_body()
{
	h_run meta
}

atf_test_case cbreak
cbreak_head()
{
	atf_set "descr" "Check cbreak mode works"
}
cbreak_body()
{
	h_run cbreak
}

atf_test_case nocbreak
nocbreak_head()
{
	atf_set "descr" "Test that the nocbreak call returns the terminal to canonical character processing"
}
nocbreak_body()
{
	h_run nocbreak
}

##########################################
# curses general attribute manipulation routines
##########################################

atf_test_case attributes
attributes_head()
{
	atf_set "descr" "Check setting, clearing and getting of attributes"
}
attributes_body()
{
	h_run attributes
}

atf_test_case wattributes
wattributes_head()
{
    atf_set "descr" ""
}
wattributes_body()
{
    h_run wattributes
}

atf_test_case getattrs
getattrs_head()
{
	atf_set "descr" "Validate curses can get and set attributes on a window"
}
getattrs_body()
{
	h_run getattrs
}

atf_test_case color_set
color_set_head()
{
    atf_set "descr" ""
}
color_set_body()
{
    h_run color_set
}

atf_test_case wcolor_set
wcolor_set_head()
{
    atf_set "descr" ""
}
wcolor_set_body()
{
    h_run wcolor_set
}

atf_test_case termattrs
termattrs_head()
{
	atf_set "descr" "Check the terminal attributes"
}
termattrs_body()
{
	h_run termattrs
}

##########################################
# curses on-screen attribute manipulation routines
##########################################

atf_test_case chgat
chgat_head()
{
	atf_set "descr" "Check changing attributes works"
}
chgat_body()
{
	h_run chgat
}

atf_test_case mvwchgat
mvwchgat_head()
{
    atf_set "descr" ""
}
mvwchgat_body()
{
    h_run mvwchgat
}

atf_test_case mvchgat
mvchgat_head()
{
	atf_set "descr" "Change the attributes on the screen"
}
mvchgat_body()
{
	h_run mvchgat
}

atf_test_case mvwchgat
mvwchgat_head()
{
    atf_set "descr" ""
}
mvwchgat_body()
{
    h_run mvwchgat
}

##########################################
# curses standout attribute manipulation routines
##########################################

atf_test_case standout
standout_head()
{
    atf_set "descr" "Checks tuning on/off of standard attribute on stdscr"
}
standout_body()
{
    h_run standout
}

atf_test_case wstandout
wstandout_head()
{
    atf_set "descr" "Checks tuning on/off of standard attribute on window"
}
wstandout_body()
{
    h_run wstandout
}

##########################################
# curses color manipulation routines
##########################################

atf_test_case has_colors
has_colors_head()
{
	atf_set "descr" "Check if the terminal can support colours"
}
has_colors_body()
{
	h_run has_colors
}

atf_test_case can_change_color
can_change_color_head()
{
	atf_set "descr" "Check if the terminal can change colours"
}
can_change_color_body()
{
	h_run can_change_color
}

atf_test_case start_color
start_color_head()
{
    atf_set "descr" ""
}
start_color_body()
{
    h_run start_color
}

atf_test_case pair_content
pair_content_head()
{
    atf_set "descr" "Checks color pair initialization and retrieval"
}
pair_content_body()
{
    h_run pair_content
}

atf_test_case init_color
init_color_head()
{
	atf_set "descr" "Set a custom color entry"
}
init_color_body()
{
	h_run init_color
}

atf_test_case color_content
color_content_head()
{
    atf_set "descr" ""
}
color_content_body()
{
    h_run color_content
}

##########################################
# curses clear window routines
##########################################

atf_test_case clear
clear_head()
{
	atf_set "descr" "Check clear,erase,clrtobot,clrtoeol work - tests window routines too"
}
clear_body()
{
	h_run clear
}

atf_test_case clearok
clearok_head()
{
    atf_set "descr" ""
}
clearok_body()
{
    h_run clearok
}

##########################################
# curses terminal update routines
##########################################

atf_test_case doupdate
doupdate_head()
{
	atf_set "descr" "Check doupdate performs an update - test wnoutrefresh too"
}
doupdate_body()
{
	h_run doupdate
}

atf_test_case immedok
immedok_head()
{
    atf_set "descr" ""
}
immedok_body()
{
    h_run immedok
}

atf_test_case leaveok
leaveok_head()
{
    atf_set "descr" "Checks cursor positioning from refresh operations - tests is_leaveok too"
}
leaveok_body()
{
    h_run leaveok
}

##########################################
# curses window scrolling routines
##########################################

atf_test_case wscrl
wscrl_head()
{
	atf_set "descr" "Check window scrolling"
}
wscrl_body()
{
	h_run wscrl
}

atf_test_case scroll
scroll_head()
{
    atf_set "descr" ""
}
scroll_body()
{
    h_run scroll
}

atf_test_case setscrreg
setscrreg_head()
{
    atf_set "descr" ""
}
setscrreg_body()
{
    h_run setscrreg
}

atf_test_case wsetscrreg
wsetscrreg_head()
{
    atf_set "descr" ""
}
wsetscrreg_body()
{
    h_run wsetscrreg
}

##########################################
# curses window modification routines
##########################################

atf_test_case touchline
touchline_head()
{
    atf_set "descr" ""
}
touchline_body()
{
    h_run touchline
}

atf_test_case touchoverlap
touchoverlap_head()
{
    atf_set "descr" ""
}
touchoverlap_body()
{
    h_run touchoverlap
}

atf_test_case touchwin
touchwin_head()
{
    atf_set "descr" ""
}
touchwin_body()
{
    h_run touchwin
}

atf_test_case untouchwin
untouchwin_head()
{
    atf_set "descr" ""
}
untouchwin_body()
{
    h_run untouchwin
}

atf_test_case wtouchln
wtouchln_head()
{
    atf_set "descr" ""
}
wtouchln_body()
{
    h_run wtouchln
}

atf_test_case is_linetouched
is_linetouched_head()
{
	atf_set "descr" "Check if a line has been modified in a window"
}
is_linetouched_body()
{
	h_run is_linetouched
}

atf_test_case is_wintouched
is_wintouched_head()
{
	atf_set "descr" "Check if a window has been modified"
}
is_wintouched_body()
{
	h_run is_wintouched
}

atf_test_case redrawwin
redrawwin_head()
{
    atf_set "descr" ""
}
redrawwin_body()
{
    h_run redrawwin
}

atf_test_case wredrawln
wredrawln_head()
{
    atf_set "descr" ""
}
wredrawln_body()
{
    h_run wredrawln
}

##########################################
# curses soft label key routines
##########################################

atf_test_case slk
slk_head()
{
    atf_set "descr" "Tests routines related to soft key labels"
}
slk_body()
{
    h_run slk
}

##########################################
# curses draw lines on windows routines
##########################################

atf_test_case hline
hline_head()
{
	atf_set "descr" "Draw a horizontal line on stdscr"
}
hline_body()
{
	h_run hline
}

atf_test_case whline
whline_head()
{
    atf_set "descr" "Draw a horizontal line on window - tests mvwhline too"
}
whline_body()
{
    h_run whline
}

atf_test_case mvhline
mvhline_head()
{
	atf_set "descr" "Move the cursor and draw a horizontal line"
}
mvhline_body()
{
	h_run mvhline
}

atf_test_case mvvline
mvvline_head()
{
	atf_set "descr" "Move the cursor and draw a vertical line"
}
mvvline_body()
{
	h_run mvvline
}

atf_test_case wvline_set
wvline_set_head()
{
    atf_set "descr" ""
}
wvline_set_body()
{
    h_run wvline_set
}

atf_test_case hline_set
hline_set_head()
{
    atf_set "descr" ""
}
hline_set_body()
{
    h_run hline_set
}

atf_test_case whline_set
whline_set_head()
{
    atf_set "descr" ""
}
whline_set_body()
{
    h_run whline_set
}

atf_test_case vline_set
vline_set_head()
{
    atf_set "descr" ""
}
vline_set_body()
{
    h_run vline_set
}

atf_test_case wvline_set
wvline_set_head()
{
    atf_set "descr" ""
}
wvline_set_body()
{
    h_run wvline_set
}

