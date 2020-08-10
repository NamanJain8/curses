# GSoC 2020 Second Evaluation Report: Curses Library Automated Testing

My GSoC project under NetBSD involves the development of test framework of curses library. This blog report is second in series of blog reports; you can have a look at the [first report](https://blog.netbsd.org/tnf/entry/gsoc_reports_curses_library_automated). This report would cover the progress made in second coding phase along with providing some insights into the libcurses. 

##  Complex characters
A complex character is a set of associated character, which may include a spacing character and non-spacing characters associated with it. Typical effects of non-spacing character on associated complex character *c* include: modifying the appearance of *c*  (like adding diacritical marks) or bridge *c* with the following character.
The **cchar_t** data type represents a complex character and its rendition. In NetBSD, this data type has following structure:
```c
struct cchar_t {
	attr_t attributes; /* character attributes */
	unsigned elements; /* number of wide char in vals*/
	wchar_t vals[CURSES_CCHAR_MAX]; /* wide chars including non-spacing */
};
```
*vals* array contains the spacing character and associated non-spacing characters. Note that NetBSD supports **wchar_t** (wide character) due to which multi-byte characters are supported. To use the complex characters one has to correctly set the locale settings. 
In this coding period, I wrote tests for routines involving complex characters.

## Alternate character set
 When you print "BSD", you would send the hex-codes 42, 53, 44 to the terminal. Capability of graphic capable printers was limited by 8-bit ASCII code. To solve this, additional character sets were introduced. We can switch between the modes using escape sequence. One such character set for Special Graphics is used by curses for line drawing. In a shell you can type 
 ```bash
 echo -e "\e(0j\e(b"
 ``` 
 to get a lower-right corner glyph. This enables alternate character mode (`\e(`), prints a character(`j`) and disables alternate character mode (`\e(b`). One might wonder where this 'j' to 'Lower Right Corner glyph' comes from. You may see that mapping ("acsc=``aaffggiijjkkllmmnnooppqqrrssttuuvvwwxxyyzz{{||}}~~,) via 
 ```bash
 infocmp -1 $TERM | grep acsc
 ````
These characters are used in `box_set()`, `border_set()`, etc. functions which I tested in the second coding period.

## Progress in the second coding phase
  
### Improvements in the framework:
1. Added support for testing of functions to be called before `initscr()`
2. Updated the unsupported function definitions with some minor bug fixes.

### Testing and bug reports
1. Added tests for following families of functions:
	- Complex character routines.
	- Line/box drawing routines.
	- Pad routines.
	- Window and sub-window operations. 
	- Curson manipulation routines
2. Reported bugs (and possible fixes if I know):
	- [lib/55454](https://gnats.netbsd.org/cgi-bin/query-pr-single.pl?number=55454) `wredrawln()` in libcurses does not follow the sensible behaviour [*fixed*]
	- [lib/55460](https://gnats.netbsd.org/cgi-bin/query-pr-single.pl?number=55460) copy error in  libcurses [*fixed*]
	- [lib/55474](https://gnats.netbsd.org/cgi-bin/query-pr-single.pl?number=55474) `wattroff()` unsets all attributes if passed STANDOUT as argument [*standard is not clear, so decided to have as it is*]
	- [lib/55482](https://gnats.netbsd.org/cgi-bin/query-pr-single.pl?number=55482) `slk_restore()` does not restore the slk screen
	- [lib/55484](https://gnats.netbsd.org/cgi-bin/query-pr-single.pl?number=55484) `newwin()` results into seg fault [*fixed*]
	- [lib/55496](https://gnats.netbsd.org/cgi-bin/query-pr-single.pl?number=55496) `bkgrnd()` doesn't works as expected
	- [lib/55517](https://gnats.netbsd.org/cgi-bin/query-pr-single.pl?number=55517) `wresize()` function incorrectly resizes the subwindows

I would like to thank my mentors Brett and Martin, as well as the NetBSD community for their support whenever I faced some issues.
