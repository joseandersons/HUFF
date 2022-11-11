all:
	gcc -o main_panel.o src/main_panel.c -c -Wall
	gcc -o list.o src/list.c -c -Wall
	gcc -o bin/teste main_panel.o list.o -Wall
	rm *.o
