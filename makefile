all: programme
	./main

test: programme
	./main test

programme: main.c
	gcc -Wall main.c -o main -lm