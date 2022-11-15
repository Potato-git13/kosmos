CFLAGS=-Wall -Wextra -Wpedantic -std=gnu17 -O3 -lreadline
LDFLAGS=-lm
CC=gcc

OUTDIR=bin
OUTNAME=$(OUTDIR)/kosmos
HEADERS=src/
FILES=$(wildcard src/*.c) $(wildcard src/*/*.c)
OBJS=$(FILES:.c=.o)

all: dir compile

dir:
	mkdir -p $(OUTDIR)

compile: $(OBJS)
	$(CC) -o $(OUTNAME) $(OBJS) $(CFLAGS) $(LDFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) -I$(HEADERS)

compile-debug: CFLAGS += -g
compile-debug: dir compile

clean:
	rm -rf $(OUTDIR) $(OBJS)

install:
	sudo cp -p $(OUTNAME) /bin/kosmos

uninstall: /bin/kosmos
	sudo rm /bin/kosmos

.PHONY: all clean compile compile-debug dir install uninstall
