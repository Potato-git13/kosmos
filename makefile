CFLAGS=-Wall -Wextra -Wpedantic -std=gnu17 -O3 -lreadline
LDFLAGS=-lm
CC=gcc

BIN=bin/
BINPATH=$(BIN)/kosmos
MAN=kosmos.1
MANPATH=/usr/local/man/man1

HEADERS=src/
FILES=$(wildcard src/*.c) $(wildcard src/*/*.c)
OBJS=$(FILES:.c=.o)

all: dirs compile

dirs:
	mkdir -p $(BIN)
	sudo mkdir -p $(MANPATH)

compile: $(OBJS)
	$(CC) -o $(BINPATH) $(OBJS) $(CFLAGS) $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) -I$(HEADERS)

compile-debug: CFLAGS += -g
compile-debug: dirs compile

clean:
	rm -rf $(BIN) $(OBJS)

install:
	sudo cp -p $(BINPATH) /bin/kosmos
	sudo cp -p $(MAN) $(MANPATH)

uninstall: /bin/kosmos
	sudo rm /bin/kosmos

.PHONY: all clean compile compile-debug dirs install uninstall
