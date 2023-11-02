# Using morse code and fork() for complicated text transfer.

The project is properly documented function by function in the morsecode.h header file. 

As a whole, the program takes one or two inputs. The first input is a filename which is passed on to the child program. The second input is the output file or - / empty for stdout. 

The child program reads the specified file, if the specified filename is "-" stdin, and sends them as morse code to the parent program. The morse code is implemented using SIGUSR1 for a short pulse or '.', SIGUSR2 for a long pulse or '-', SIGALRM for a ' ' in text and SIGINT to indicate a character ended. The program does hence not terminate from ctrl+c.

The parent program just recieves the signals, processes them and confirms it is ready to the child.

The program ignores signals it can't desipher meaning characters not in the following list: ABCDEFGHIJLKMNOPQRSTUVWXYZ1234567890&'@():,=!.-+"?/\n

Usage:
To compile the program using the makefile: 
```
make 
```

To run the program with input testfile1234 and output stdout:
```
make run
```

To delete compiled program and log file:
```
make clean
```

To run default with valgrind:
```
make valgrind
```

Otherwise, after compiling with "make":
```
./morsecode.o <input file or '-' for stdin> <output file or '-' / empty for stdout>
```