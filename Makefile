all:
	gcc -O3 -o bin/cafezip src/helpful.c src/compress.c src/tree.c src/main_panel.c src/list.c -Wall
clean:
	rm bin/cafezip