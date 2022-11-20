all:
	gcc -std=c17 src/tree.c -o bin/tree.o -c
	gcc -std=c17 src/avl.c -o bin/avl.o -c 
	gcc -std=c17 src/main.c -o bin/main.o -c -I "/home/higor/MATLAB/R2022b/extern/include/"
	gcc -std=c17 bin/tree.o bin/main.o bin/avl.o -L "/home/higor/MATLAB/R2022b/bin/glnxa64" -Wl,-rpath-link=/home/higor/MATLAB/R2022b/bin/glnxa64 -lmx -leng -lmat -lmex -lut -o bin/plot
	rm bin/*.o