CC = gcc
CFLAGS = -std=c99 -O0 -ggdb -Wall -Werror
EXEC = libcfg_example

all:
	$(CC) $(CFLAGS) -o $(EXEC) ../libcfg.c example.c -I..

clean:
	rm $(EXEC)
