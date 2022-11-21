all:
	gcc -std=c17 src/tree.c -o bin/tree.o -c
	gcc -std=c17 src/avl.c -o bin/avl.o -c 
	gcc -std=c17 src/main.c -o bin/main.o -c -I "/usr/local/MATLAB/R2022b/extern/include"  -fexceptions -fno-omit-frame-pointer -O2 -fwrapv -DNDEBUG
	gcc -std=c17 bin/tree.o bin/main.o bin/avl.o  -L "/usr/local/MATLAB/R2022b/bin/glnxa64" -m64 -Wl,--no-undefined -s -Wl,-rpath-link=/usr/local/MATLAB/R2022b/bin/glnxa64 -lmx -leng -lmat -lmex -lut -o bin/plot
	rm bin/*.o

win:
	gcc -std=c11 src\tree.c -o bin\tree.obj -c
	gcc -std=c11 src\avl.c -o bin\avl.obj -c 
	gcc -c -m64 -DUSE_MEX_CMD -I"C:\MATLAB/extern/include" -I"C:\MATLAB/simulink/include" -I"C:\MATLAB/extern\lib\win64\mingw64" -fexceptions -fno-omit-frame-pointer -O2 -fwrapv -DNDEBUG src\main.c -o bin\main.obj
	gcc -m64 -Wl,--no-undefined -static -s  bin\tree.obj bin\avl.obj bin\main.obj -L"C:\MATLAB\extern\lib\win64\mingw64" -llibmx -llibmex -llibmat -lm -llibmwlapack -llibmwblas -llibeng -o bin\plot.exe
	del bin\*.obj