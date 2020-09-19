  
# Google Summer of Code 2020: [Final Report] Curses Library Automated Testing

My GSoC project under NetBSD involves the development of the test framework of curses. This is the final blog report in a series of blog reports; you can look at the [first report](https://blog.netbsd.org/tnf/entry/gsoc_reports_curses_library_automated) and [second report](https://blog.netbsd.org/tnf/entry/gsoc_2020_second_evaluation_report) of the series.

The first report gives a brief introduction of the project and some insights into the curses testframe through its architecture and language. To someone who wants to contribute to the test suite, this blog can act as the quick guide of how things work internally. Meanwhile, the second report discusses some of the concepts that were quite challenging for me to understand. I wanted to share them with those who may face such a challenge. Both of these reports also cover the progress made in various phases of the Summer of Code.

This being the final report in the series, I would love to share my experience throughout the project. I would be sharing some of the learning as well as caveats that I faced in the project.

## Challenges and Caveats
By the time my application for GSoC was submitted, I had gained some knowledge about the curses library and the testing framework. Combined with compiler design and library testing experience, that knowledge proved useful but not sufficient as I progressed through the project. There were times when, while writing a test case, you have to look into documentation from various sources, be it NetBSD, FreeBSD, Linux, Solaris, etc. One may find questioning his understanding of the framework, documentation, or even curses itself. This leads to the conclusion that for being a tester, one has to become a user first. That made me write minimal programs to understand the behavior. The experience was excellent, and I felt amazed by the capability and complexity of curses.

## Learnings
The foremost learning is from the experience of interacting with the open-source community and feeling confident in my abilities to contribute. Understanding the workflows; following the best practices like considering the maintainability, readability, and simplicity of the code were significant learning.

The project-specific learning was not limited to test-framework but a deeper understanding of curses as I have to browse through codes for the functions tested. As [this](https://www.linusakesson.net/programming/tty/) blog says, getting the TTY demystified was a long-time desire, which got fulfilled to some extent.

## Some tests from test suite
In this section, I would discuss a couple of tests of the test suite written during the third phase of GSoC. Curses input model provides a variety of ways to obtain input from keyboard. We will consider 2 tests `keypad` and `halfdelay` that belong to input processing category but are somewhat unrelated. 

### Keypad Processing
An application can enable or disable the tarnslation of keypad using `keypad()` function. When translation is enabled, curses attempts to translate input sequence into a single key code. If disabled, curses passes the input as it is and any interpretation has to be made by application.
```
include window
call $FALSE is_keypad $win1
input "\eOA"
call 0x1b wgetch $win1
call OK keypad $win1 $TRUE
input "\eOA"
call $KEY_UP wgetch $win1

# disable assembly of KEY_UP
call OK keyok $KEY_UP $FALSE
input "\eOA"
call 0x1b wgetch $win1
``` 
As keypad translation is disabled by default, on input of '\eOA', the input sequence is passed as it is and only '\e' (0x1b is hex code) is received on `wgetch()`. If we enable the translation, then the same input is translated as KEY_UP. In curses, one can disable assembly of specific key symbols using `keyok()`. See related [man page](http://man.netbsd.org/keypad.3).

### Input Mode
Curses lets the application control the effect of input using four input modes; cooked, cbreak, half-delay, raw. They specify the effect of input in terms of echo-ing and delay. We will discuss about the `halfdelay` mode. The half-delay mode specifies how quickly certain curses function return to application when there is no terminal input waiting since the function is called.
```
include start
delay 1000
# input delay 1000 equals to 10 tenths of seconds
# getch must fail for halfdelay(5) and pass for halfdelay(15)
input "a"
call OK halfdelay 15
call 0x61 getch
call OK halfdelay 5
input "a"
call -1 getch
```
We have set the delay for feeding input to terminal with delay of 1s(10 tenths of second). If the application sets the halfdelay to 15, and makes a call to `getch()` it receives the input. But it fails to get the input with haldelay set to 5. See related [man page](http://man.netbsd.org/halfdelay.3).

## Project Work
The [work](https://github.com/NamanJain8/curses) can be merged into organisation repository [https://github.com/NetBSD/src](https://github.com/NetBSD/src) under [tests/lib/libcurses](https://github.com/NetBSD/src/tree/trunk/tests/lib/libcurses).

This project involved:
 1. Improvement in testframework:
 	- Automation of the checkfile generation.
	- Enhnacement of support for complex character
	- Addition of small features and code refactoring
 2. Testing and bug reports:
	- Tests for a family of routines like wide character, complex character, line drawing, box drawing, pad, window operations, cursor manipulations, soft label keys, input-output stream, and the ones involving their interactions.
	- Raising a bunch of Problem Report (PR) under `lib` category some of which have been fixed. The list of PRs raised can be found [here](https://github.com/NamanJain8/curses/blob/master/reports/problem-reports.md)

## Future Work
- The current testframe supports complex character, but the support needs to be extended for its string. This will enable testing of `[mv][w]add_wch[n]str`, `[mv][w]in_wchstr` family of routines.
- Some of the tests for teminal manipulation routines like `intrflush`, `def_prog_mode`, `typeahead`, `raw`, etc. are not there in test suite.
-  Not specifically related to the framework, but the documentation for wide character as well as complex character routines need to be added.

## Acknowledgements
I want to extend my heartfelt gratitude to my mentor Mr. Brett Lymn, who helped me through all the technical difficulties and challenges I faced. I also thank my mentor Martin Huseman for valuable suggestions and guidance at various junctures of the project. A special thanks to Kamil Rytarowski for making my blogs published on the NetBSD site.