CC=gcc
CFLAGS=-Wall -g -O
LFLAGS=-lncurses
PROGS=main

all: $(PROGS)

main: main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean:
	/bin/rm -f $(PROGS) *.o a.out *.d
	/bin/rm -fr *.dSYM
