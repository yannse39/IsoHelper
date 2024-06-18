CC = gcc
CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla
BIN = iso_helper

all:	$(BIN)

$(BIN):	src/iso9660.o	src/main.o
	$(CC)	$(CFLAGS)	-o	$(BIN)	src/iso9660.o	src/main.o

src/iso9660.o:	src/iso9660.c 
	$(CC)	$(CFLAGS)	-c	src/iso9660.c	-o	src/iso9660.o

src/main.o:	src/main.c 
	$(CC)	$(CFLAGS)	-c	src/main.c	-o	src/main.o

test:	$(BIN)
	./$(BIN)	example.iso	<	commands

clean:
	rm	-f	src/*.o	$(BIN)

.PHONY: all clean test
