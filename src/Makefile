CC=gcc
CFLAGS= -std=gnu11 -g -O3 -Wall -Wshadow -Wextra -Wpedantic
LIBS=
INSTALL_PATH?=/usr/local

.PHONY: all clean cls install

all: clean cls

clean:
	rm -f ../bin/*

cls:
	$(CC) -o ../bin/cls main.c $(CFLAGS) $(LIBS)

install: ../bin/cls
	mkdir -p $(INSTALL_PATH)/bin && cp /home/beronthecolossus/Peki/C/Projects/cls/bin/cls $(INSTALL_PATH)/bin/cls

