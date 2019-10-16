CC=gcc
CFLAGS=
LIBS=

all: clean cls

clean:
	rm -f ../bin/cls
cls:
	$(CC) -o ../bin/cls main.c $(CFLAGS) $(LIBS) -O3
install:
	sudo ln -sf ~/Peki/C/Projects/cls/bin/cls /usr/bin/cls
