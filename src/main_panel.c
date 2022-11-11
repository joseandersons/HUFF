#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
//..//
#include "../include/main_panel.h"
#include "../include/lista.h"


void usage(){
	printf("Welcome to %s\n"
		   "Usage: %s [OPTIONS] [FILE]\n"
		   "\nExample: %s -c image.png\n"
		   "\nOptions: \n"
		   "  -x, --extract       Extract a file .huff file\n"
		   "  -c, --compress      Compress any file to .huff file\n"
		   "  -h, --help          Show this message\n",
		   PROGRAM, PROGRAM, PROGRAM);
}

short parse_options(char *argv[]){
	if(!strcmp(argv[1], "-x") || !strcmp(argv[1], "--extract")){
		return 1;
	}else if(!strcmp(argv[1], "-c") || !strcmp(argv[1], "--compress")){
		return 2;
	}else if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")){
		return 3;
	}else{
		return 0;
	}
}

_Bool select_options(short options, char *name){
	if(options == 1){
		int fd = open(name, O_RDONLY);
		
		if(fd == -1){
			puts("Error: File doesn't exist or no permissions");
			return 0;
		}

		return 1;
	}else if(options == 2){
		int fd = open(name, O_RDONLY);

		if(fd == -1){
			puts("Error: File doesn't exist or no permissions");
			return 0;
		}

		printf("Compactando...\n");
		return 1;
	}else if(options == 3){
		usage();
		return 1;
	}else{
		usage();
	}

	return 0;
}

int main(int argc, char *argv[]){
	if(argc < 3){
		usage();
		return EXIT_FAILURE;
	}

	short option = parse_options(argv);

	if(!option){
		usage();
		return EXIT_FAILURE;
	}

	_Bool status = select_options(option, argv[2]);

	return !status;
}