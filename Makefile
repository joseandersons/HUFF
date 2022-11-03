all:
	gcc -o main.o src/main.c -c -Wall
	gcc -o pilha.o src/pilhas.c -c -Wall
	gcc -o bin/teste main.o pilha.o -Wall
	rm *.o
