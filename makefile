CFLAGS=-Wall -Wextra -Wpedantic -std=gnu17 -O3 -lreadline
CC=gcc

clean:
	rm -rf bin/

compile:
	mkdir -p bin/
	$(CC) -o bin/kosmos src/main.c $(CFLAGS)

compile-debug: CFLAGS += -g
compile-debug: compile

run:
	bin/kosmos

install:
	sudo cp -p bin/kosmos /bin/kosmos

uninstall:
	sudo rm /bin/kosmos

.PHONY: clean compile compile-debug run install uninstall
