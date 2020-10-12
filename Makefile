main: main.c
	gcc main.c -o main -lncurses


run: main
	./main
