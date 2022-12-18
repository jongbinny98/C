##################################################
# C compiler

CC = gcc

# C compiler flags

CFLAGS = -g -Wall -Werror -Wextra -pedantic


# all: queue


# # Used to build executable from .o file(s)

# queue: queue.o

# 	$(CC) $(CFLAGS) -o queue queue.o


# Used to build .o file(s) from .c files(s)

queue.o: queue.c

	$(CC) $(CFLAGS) -c queue.c


# clean built artifacts

clean:

	rm -f queue 

