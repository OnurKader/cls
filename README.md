# My `list` program written in C

## _'cls'_ = Color List | _C-LS_, due to it being written in _C_

## Screenshots

**_TODO_**
___

## Build & Install

To build, run `make` in the `src/` directory.

To install, run `sudo make install`, it installs in `bin/` and copies to `/usr/local/bin/cls`
by default but the installation location can be changed by modifying the **Makefile**

```make
CC=gcc
CFLAGS=...
LIBS=
INSTALL_PATH?=/usr
```

Will install it as `/usr/bin/cls`

## Usage

`cls` doesn't have as many features as the default `GNU ls` but it can do
simple listings

`cls` will just print non-hidden, files which do not start with a period '.'

`cls -a || cls --all` will print hidden files, but it will ignore
`.` and `..` by default

`cls -1 || cls --one-line` will print all the listings on a newline

`cls -l || cls --long` will print extra information about the file / dir **_TODO_**

`cls -Huv || cls --help --usage --version` to get a simple help screen printed out

`cls -r || cls --reverse` will reverse the printing **_TODO_**

