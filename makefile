CFLAGS=-Wall -Wextra -Wpedantic -std=gnu17 -O3 -lreadline
CC=gcc

OUTDIR=bin
OUTNAME=$(OUTDIR)/kosmos
FILES=src/main.c
HEADERS=src/headers/

clean: $(OUTDIR)
	rm -rf $(OUTDIR)

compile: $(FILES)
	mkdir -p $(OUTDIR)
	$(CC) -o $(OUTNAME) $(FILES) -I$(HEADERS) $(CFLAGS)

compile-debug: CFLAGS += -g
compile-debug: compile

run: compile $(OUTNAME)
	$(OUTNAME)

install: /bin/kosmos
	sudo cp -p $(OUTNAME) /bin/kosmos

uninstall: /bin/kosmos
	sudo rm /bin/kosmos

.PHONY: clean compile compile-debug run install uninstall
