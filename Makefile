CC=gcc
CFLAGS=-Wall -g -O
PROGS=main

all: $(PROGS)

clean:
	/bin/rm -f $(PROGS) *.o a.out *.d
	/bin/rm -fr *.dSYM
