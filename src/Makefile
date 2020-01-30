CC=gcc
CFLAGS= -std=gnu11 -g -O3 -Wall -Wshadow -Wextra -Wpedantic
LIBS=

.PHONY: all clean cls install

all: clean cls

clean:
	rm -f ../bin/*

cls:
	$(CC) -o ../bin/cls main.c $(CFLAGS) $(LIBS)

install: ../bin/cls
	cp /home/beronthecolossus/Peki/C/Projects/cls/bin/cls /usr/local/bin/cls
