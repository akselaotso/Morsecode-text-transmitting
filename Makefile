APPLICATION = morsecode.o
SRC_FILES = main.c child_process.c functions/*.c
HEADER_FILES = morsecode.h
CC = cc
CFLAGS = -O2 -g -Wall -pedantic
LOGFILE = log_file.txt

all: $(APPLICATION)

$(APPLICATION): $(SRC_FILES) $(HEADER_FILES)
	$(CC) $(CFLAGS) -o $(APPLICATION) $(SRC_FILES)

clean:
	rm -f $(APPLICATION) $(LOGFILE)

valgrind: $(APPLICATION)
	valgrind --leak-check=full --show-leak-kinds=all ./$(APPLICATION) testfile1234

run: $(APPLICATION)
	./$(APPLICATION) testfile1234

