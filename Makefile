APPLICATION = morsecode.o
SRC_FILES = main.c child_process.c functions/*.c
HEADER_FILES = morsecode.h
CC = cc
CFLAGS = -O2 -g -Wall -pedantic
LOGFILE = log_file.txt

all: main

main: $(SRC_FILES) $(HEADER_FILES)
	$(CC) $(CFLAGS) -o $(APPLICATION) $(SRC_FILES)

clean:
	rm -f $(APPLICATION) $(LOGFILE)

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all ./$(APPLICATION) -

run:
	./$(APPLICATION) testfile1234

