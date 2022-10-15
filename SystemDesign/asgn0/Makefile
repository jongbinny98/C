##################################################
# C compiler

CC = gcc

# C compiler flags

CFLAGS = -g -Wall -Werror -Wextra -pedantic


all: split


# Used to build executable from .o file(s)

split: split.o

	$(CC) $(CFLAGS) -o split split.o


# Used to build .o file(s) from .c files(s)

split.o: split.c

	$(CC) $(CFLAGS) -c split.c


# clean built artifacts

clean:

	rm -f split split.o
