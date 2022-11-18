#include "../include/main_panel.h"
//...//
#include "../include/helpful.h"
#include "../include/list.h"
#include "../include/compress.h"
#include "../include/decompress.h"

void usage(){
	printf(RESET WHITE_INTENSE "Welcome to "
		   BLINK ORANGE "%s\n" 
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

char *get_new_name(int argc, char *argv[], short options){
	char *new_file = NULL;

	if(argc == 4){
		char *aux = argv[3];
		new_file = malloc(strlen(aux) + 5);
		if(!new_file)
			return NULL;

		new_file = strcpy(new_file, argv[3]);
	}else{
		char *cpy, *state, *aux;

		cpy = strdup(argv[2]);
		if(!cpy)
			return NULL;

		aux = strtok_r(cpy, ".", &state);

		new_file = malloc(strlen(aux) + 5);
		if(!new_file)
			return NULL;

		new_file = strcpy(new_file, aux);

		free(cpy);
	}

	if(options == 2){
		new_file = strcat(new_file, ".huff");
	}

	return new_file;
}

_Bool select_options(int argc, char *argv[], short options){
	char *file_name = argv[2];
	char *new_file = NULL;
	_Bool status = 0;
	int new_fd;

	int fd = open(file_name, O_RDONLY);
	if(fd == -1){
		error("Error: File doesn't exist or permission denied!");
		return 0;
	}

	new_file = get_new_name(argc, argv, options);
	if(!new_file)
		return 0;

	new_fd = open(new_file, O_WRONLY | O_CREAT, 0764);
	if(new_fd == -1){
		error("Error: File doesn't exist or permission denied!");
		close(fd);
		return 0;
	}

	if(options == 1){
		printf(WHITE "Decompressing...\n");

		_Bool status = decompress(fd, new_fd);
		if(!status){
			error("Decompression failed!");
		}else{
			printf(GREEN "Successfully decompressed!\n");
		}
	}else if(options == 2){
		printf(WHITE "Compressing...\n");

		_Bool status = compress(fd, new_fd);
		if(!status){
			error("Compression failed!");
		}else{
			printf(GREEN "Successfully compressed!\n");
		}
	}else if(options == 3){
		usage();
		status = 1;
	}else{
		usage();
		status = 0;
	}

	close(fd);
	close(new_fd);
	free(new_file);
	return status;
}

int main(int argc, char *argv[]){
	if(argc < 3){
		usage();
		return EXIT_FAILURE;
	}

	if(argc > 4){
		usage();
		return EXIT_FAILURE;
	}

	short option = parse_options(argv);
	if(!option){
		usage();
		return EXIT_FAILURE;
	}

	_Bool status = select_options(argc, argv, option);
	if(!status)
		return EXIT_FAILURE;

	return 0;
}