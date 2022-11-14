#include "../include/main_panel.h"
//...//
#include "../include/helpful.h"
#include "../include/list.h"
#include "../include/compress.h"

void usage(){
	printf(RESET WHITE_INTENSE "Welcome to "
		   BLINK "%s\n" 
		   RESET WHITE_INTENSE "\nUsage:   %s [OPTION] [FILE]"
		   RESET ITALIC WHITE "\nExample: %s -c image.png\n"
		   RESET WHITE_INTENSE "\nOptions: \n"
		   RESET YELLOW "  -x, --extract       "
		   RESET BLUE "Extract a .huff file\n"
		   RESET YELLOW "  -c, --compress      "
		   RESET BLUE "Compress any file to .huff file\n"
		   RESET YELLOW "  -h, --help          "
		   RESET BLUE "Show this message\n",
		   PROGRAM, PROGRAM_LOWERCASE, PROGRAM_LOWERCASE);
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
	int fd = open(name, O_RDONLY);

	if(fd == -1){
		error("Error: File doesn't exist or permission denied!");
		return 0;
	}

	if(options == 1){
		printf("Descompactando...\n");

		//_Bool status = decompress(fd);

		//return status;
	}else if(options == 2){
		printf("Compressing...\n");

		_Bool status = compress(fd, name);
		if(!status){
			error("Compression failed!");
			return 0;
		}

		printf("Successfully compressed!\n");
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
	if(!status)
		return EXIT_FAILURE;

	return 0;
}