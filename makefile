CFLAGS=-Wall -Wextra -Wpedantic -std=gnu17 -O3 -lreadline
LDFLAGS=-lm
CC=gcc

BIN=bin
BINPATH=$(BIN)/kosmos
HEADERS=src/
FILES=$(wildcard src/*.c) $(wildcard src/*/*.c)
OBJS=$(FILES:.c=.o)

all: dir compile

dir:
	mkdir -p $(BIN)

compile: $(OBJS)
	$(CC) -o $(BINPATH) $(OBJS) $(CFLAGS) $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) -I$(HEADERS)

compile-debug: CFLAGS += -g
compile-debug: dir compile

clean:
	rm -rf $(BIN) $(OBJS)

install:
	sudo cp -p $(BINPATH) /bin/kosmos

uninstall: /bin/kosmos
	sudo rm /bin/kosmos

.PHONY: all clean compile compile-debug dir install uninstall
