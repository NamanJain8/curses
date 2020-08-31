# Google Summer of Code 2020 - "Curses Library Automated Testing"
The repository contains the code created for the project **Curses Library Automated Testing** during the Google Summer of Code 2020, under the aegis of **The NetBSD Foundation**.

## Overview - Curses framework
This section briefly describes the curses library and its testing framework.

### Curses Library
Curses is a terminal control library for the NetBSD system, enabling the development of TUI applications without writing directly for any specific terminal type. Based on the terminal type it sends the correct control character. NetBSD curses follows SUSv2 (The Single Unix Specification, Version 2) specification with some additional ncurses extension. It is an important part of NetBSD OS with many applications relying on it. The library supports wide-characters and complex characters as well. Performing modifications on the curses library can be difficult because the effects of the change may be subtle and can introduce bugs that are not detected for a long time.
 
### Test Framework
Automated Test Framework (ATF) was introduced in 2007 but ATF cannot be used directly for curses testing for several reasons. Hence, we have a separate test framework in place for testing curses. This framework was built for NetBSD by Brett Lymn.

The testframework consists of 2 programs, director and slave. The framework provides its own simple language for writing tests. The slave is a curses application capable of running any curses function, while the director acts as a coordinator and interprets the test file and drives the slave program. The director can also capture slave's output which can be used for comparison with desired output.
![Test Framework](https://raw.githubusercontent.com/NamanJain8/curses/master/reports/director-slave.jpeg)
The director forks a process operating in pty and executes a slave program on that fork. The master side of pty is used for getting the data stream that the curses function call produces which can be futher used to check the correctness of behaviour. Director and slave communicate via pipes; command pipe and slave pipe. The command pipe carries the function name and arguments, while slave pipe carries return codes and values from function calls.

### Test Language
The tests have to be written in a custom language defined by a formal grammar which is parsed by the director. The test language comprises a small number of commands, like assign, call, check which are described in the [libcurses testframe](https://github.com/NetBSD/src/blob/trunk/tests/lib/libcurses/testframe.txt). The framework supports 3 kind of strings; null terminated string, byte string, and string of type chtype, based on the quotes enclosing it. 
```
include start
call win newwin 10 20 2 5
check win NON_NULL
call OK waddstr $win "Hello World!"
call OK wrefresh $win
compare waddstr_refresh.chk
```
This is a basic program which initialises the screen, creates new window, checks if the window creation was successful, adds as string "Hello World!" on the window, refreshes the window, and compares it with desired output stored in check file.

## Project Work
This work can be merged into organisation repository [https://github.com/NetBSD/src](https://github.com/NetBSD/src) under [tests/lib/libcurses](https://github.com/NetBSD/src/tree/trunk/tests/lib/libcurses).

This project involved:
 1. Improvement in testframework:
 	- Automation of the checkfile generation.
	- Enhnacement of support for complex character
	- Addition of small features and code refactoring
 2. Testing and bug reports:
	- Tests for a family of routines like wide character, complex character, line drawing, box drawing, pad, window operations, cursor manipulations, soft label keys, input-output stream, and the ones involving their interactions.
	- Raising a bunch of Problem Report (PR) under `lib` category some of which have been fixed. The list of PRs raised can be found [here](https://github.com/NamanJain8/curses/blob/master/reports/problem-reports.md)

## Future Work
- The current testframe supports complex character, but the support needs to be extended for its string. This will enable testing of [mv][w]add_wch[n]str, [mv][w]in_wchstr family of routines.
- Some of the tests for teminal manipulation routines like intrflush, def_prog_mode, typeahead, raw, etc. are not there in test suite.
-  Not specifically related to the framework, but the documentation for wide character as well as complex character routines need to be added.

## Acknowledgements
I would like to extend my heart-felt gratitude to my mentor Mr. Brett Lymn, who helped me though all the technical difficulties and challenges I faced regarding the project. Needless to say, without his guidance, this work won't be in its current state. I would also like to thank my mentor Martin Huseman for valuable suggestions and guidance at various junctures of project. I would also like to specially thank Kamil Rytarowski for making my blogs publish on NetBSD site. Lots of thanks to Google for sponsoring and NetBSD Foundation for funding the project in terms of valuable support from community and team. 

## Conclusion
Google Summer of Code 2020 has been a unique experience for me. This has provide me the thrust into Open-Source Community and helped me understand its workings. Now, I feel more confident in my abilities towards contributing to Open Source and more excited to make fruitful contributions in future. I would like to thank Google for providing this wonderful platform that helped me spend my Summers in the best way possible!
