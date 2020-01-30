CC=gcc
CFLAGS= -std=gnu11 -g -O3 -Wall -Wshadow -Wextra -Wpedantic
LIBS=
INSTALL_PATH?=/usr/local

.PHONY: all clean cls install

all: clean cls

clean:
	rm -f ../bin/*

cls: main.c color.c icons.c
	mkdir -p ../bin && $(CC) -o ../bin/cls main.c $(CFLAGS) $(LIBS)

install: ../bin/cls
	mkdir -p $(INSTALL_PATH)/bin && cp ../bin/cls $(INSTALL_PATH)/bin/cls

