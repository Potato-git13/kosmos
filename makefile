CFLAGS=-Wall -Wextra -Wpedantic -std=c17 -O3 -lreadline
CC=gcc
compile:
	mkdir -p bin/
	$(CC) -o bin/kosmos src/main.c $(CFLAGS)
compile-debug:
	mkdir -p bin/
	$(CC) -o bin/kosmos src/main.c -g $(CFLAGS)
run:
	bin/kosmos
push-to-user-bin:
	cp -p bin/kosmos ${HOME}/bin/kosmos
remove-from-user-bin:
	rm ${HOME}/bin/kosmos
push-to-global-bin:
	sudo cp -p bin/kosmos /bin/kosmos
remove-from-global-bin:
	sudo rm /bin/kosmos
