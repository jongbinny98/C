##################################################
# C compiler

CC = gcc

# C compiler flags

CFLAGS = -g -Wall -Werror -Wextra -pedantic


all: httpserver


# Used to build executable from .o file(s)

httpserver: httpserver.o

	$(CC) $(CFLAGS) -o httpserver httpserver.o


# Used to build .o file(s) from .c files(s)

httpserver.o: httpserver.c

	$(CC) $(CFLAGS) -c httpserver.c


# clean built artifacts

clean:

	rm -f httpserver httpserver.o

