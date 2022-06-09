CFLAGS=-Wall -Wextra -Wpedantic -std=gnu17 -O3 -lreadline
CC=gcc

OUTDIR=bin/
KOSMOS=bin/kosmos
FILES=src/main.c
HEADERS=src/headers/

clean:
	rm -rf $(OUTDIR)

compile:
	mkdir -p $(OUTDIR)
	$(CC) -o $(KOSMOS) $(FILES) -I$(HEADERS) $(CFLAGS)

compile-debug: CFLAGS += -g
compile-debug: compile

run:
	$(KOSMOS)

install:
	sudo cp -p $(KOSMOS) /bin/kosmos

uninstall:
	sudo rm /bin/kosmos

.PHONY: clean compile compile-debug run install uninstall
