CFLAGS=-Wall -Wextra -Wpedantic -std=gnu17 -O3 -lreadline
CC=gcc

OUTDIR=bin
FILES=src/main.c
HEADERS=src/headers/

clean:
	rm -rf $(OUTDIR)

compile:
	mkdir -p $(OUTDIR)
	$(CC) -o $(OUTDIR)/kosmos $(FILES) -I$(HEADERS) $(CFLAGS)

compile-debug: CFLAGS += -g
compile-debug: compile

run: 
	$(OUTDIR)/kosmos

install:
	sudo cp -p $(OUTDIR)/kosmos /bin/kosmos

uninstall:
	sudo rm /bin/kosmos

.PHONY: clean compile compile-debug run install uninstall
