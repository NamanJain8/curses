# GSoC 2020 First Evaluation Report: Curses Library Automated Testing

## Introduction

My GSoC project under NetBSD involves the development of test framework of curses library. Automated Test Framework (ATF) was introduced in 2007 but ATF cannot be used directly for curses testing for several reasons most important of them being curses has functions which do timed reads/writes which is hard to do with just piping characters to test applications. Also, stdin is not a tty device and behaves differently and may affect the results. A lot of work regarding this has been done and we have a separate test framework in place for testing curses. 

The aim of project is to build a robust test suite for the library and complete the SUSv2 specification. This includes writing tests for the remaining functions and enhancing the existing ones. Meanwhile, the support for complex character function has to be completed along with fixing some bugs, adding features and improving the test framework. 


## Why did I chose this project?

I am a final year undergraduate at Indian Institute of Technology, Kanpur. I have my majors in Computer Science and Engineering, and I am specifically interested in algorithms and computer systems. I had worked on building and testing a library on Distributed Tracing at an internship and understand the usefulness of having a test suite in place. Libcurses being very special in itself for the testing purpose interested me. Knowing some of the concepts of  compiler design made my interest a bit more profound.

## Test Framwork

The testframework consists of 2 programs, director and slave. The framework provides its own simple language for writing tests. The slave is a curses application capable of running any curses function, while the director acts as a coordinator and interprets the test file and drives the slave program. The director can also capture slave's output which can be used for comparison with desired output.

The director forks a process operating in pty and executes a slave program on that fork. The master side of pty is used for getting the data stream that the curses function call produces which can be futher used to check the correctness of behaviour. Director and slave communicate via pipes; command pipe and slave pipe. The command pipe carries the function name and arguments, while slave pipe carries return codes and values from function calls. 

Let's walk through a sample test to understand how this works. Consider a sample program:

```
include start
call win newwin 2 5 2 5
check win NON_NULL
call OK waddstr $win "Hello World!"
call OK wrefresh $win
compare waddstr_refresh.chk

```

This is a basic program which initialises the screen, creates new window, checks if the window creation was successful, adds as string "Hello World!" on the window, refreshes the window, and compares it with desired output. The details of the language can be accessed at [libcurses testframe](https://github.com/NetBSD/src/blob/trunk/tests/lib/libcurses/testframe.txt).

The test file is interpreted by the language parser and the correponding actions are taken. Let's look how line #2 is processed. This command creates a window using `newwin()`. The line is ultimately parsed as [`call: CALL result fn_name args eol`](https://github.com/NetBSD/src/blob/1d30657e76c9400c15eb4e4cfcdff2fea6c65a5a/tests/lib/libcurses/director/testlang_parse.y#L237)grammar rule and executes the function [`do_funtion_call()`](https://github.com/NetBSD/src/blob/1d30657e76c9400c15eb4e4cfcdff2fea6c65a5a/tests/lib/libcurses/director/testlang_parse.y#L1035)). Now, this function [sends](https://github.com/NetBSD/src/blob/1d30657e76c9400c15eb4e4cfcdff2fea6c65a5a/tests/lib/libcurses/director/testlang_parse.y#L1048) function name and arguments using command pipe to the slave. The slave, who is waiting to get command from the director, reads from the pipe and [executes](https://github.com/NetBSD/src/blob/1d30657e76c9400c15eb4e4cfcdff2fea6c65a5a/tests/lib/libcurses/slave/slave.c#L144) the command. This executes the correponding curses function from the [command table](https://github.com/NetBSD/src/blob/1d30657e76c9400c15eb4e4cfcdff2fea6c65a5a/tests/lib/libcurses/slave/command_table.h#L40) and the pointer to new window is [returned](https://github.com/NetBSD/src/blob/1d30657e76c9400c15eb4e4cfcdff2fea6c65a5a/tests/lib/libcurses/slave/curses_commands.c#L3582) via the slave pipe ([here](https://github.com/NetBSD/src/blob/1d30657e76c9400c15eb4e4cfcdff2fea6c65a5a/tests/lib/libcurses/slave/commands.c#L167)) after passing wrappers of functions. The director [recieves](https://github.com/NetBSD/src/blob/1d30657e76c9400c15eb4e4cfcdff2fea6c65a5a/tests/lib/libcurses/director/testlang_parse.y#L1140) them, and returned value is assigned to a variable(`win` in line#2) or compared (`OK` in line#4). This is the typical life cycle of a certain function call made in tests.

Along with these, the test framework provides capability to `include` other test (line#1), `check` the variable content (line#3), `compare` the data stream due to function call in pty with desired stream (line#6). Tester can also provide inputs to functions via `input` directive, perform delay via `delay` directive, assign values to variables via `assign` directive, and create a wide or complex charater via `wchar` and `cchar` directives respectively. 

## Progress till the first evaluation

1. Completed the support for complex chacter tests in director and slave.
2. Automated the checkfile generation that has to be done manually earlier.
3. Wrote tests for wide character routines.
4. Refactored the code.
5. Reported bugs:
	- [lib/55433](https://gnats.netbsd.org/cgi-bin/query-pr-single.pl?number=55433) Bug in special character handling of ins_wstr() of libcurses
	- [lib/55434](https://gnats.netbsd.org/cgi-bin/query-pr-single.pl?number=55434) Bug in hline() in libcurses [fixed]
	- [lib/55443](https://gnats.netbsd.org/cgi-bin/query-pr-single.pl?number=55443) setcchar() incorrectly sets the number of elements in cchar [fixed]
