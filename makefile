CFLAGS=-g -Wall -Wextra -Wpedantic -std=c11 -O3 -lreadline
compile:
	mkdir -p bin/
	gcc -o bin/kosmos src/main.c $(CFLAGS)
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
