CFLAGS=-Wall -Wextra -Wpedantic -std=gnu17 -O3 -lreadline
CC=gcc

OUTDIR=bin
OUTNAME=$(OUTDIR)/kosmos
HEADERS=src/headers/
FILES=$(wildcard src/*.c)
OBJS=$(FILES:.c=.o)

all: dir compile

dir:
	mkdir -p $(OUTDIR) $(OBJS)

compile: $(FILES)
	$(CC) -o $(OUTNAME) $(FILES) -I$(HEADERS) $(CFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS) -I$(HEADERS)

compile-debug: CFLAGS += -g
compile-debug: dir compile

clean: $(OUTDIR)
	rm -rf $(OUTDIR)

install:
	sudo cp -p $(OUTNAME) /bin/kosmos

uninstall: /bin/kosmos
	sudo rm /bin/kosmos

.PHONY: all clean compile compile-debug dir install uninstall
