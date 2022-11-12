#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
//..//
#include "../include/lista.h"
#include "../include/tree.h"

#define BLOCK_SIZE 8192 //8 KiB

/*struct data{
	void *data_1;
  	void *data_2;
};*/

void test(struct data *dados){
	uint64_t *recurrence;

	recurrence = (uint64_t *)dados->data_1;

	int soma = 0;
	printf("Recurrence: \n");
	for(int c = 0; c < 256; c ++){
		soma += recurrence[c];
		printf("[%x]:[%ld]\n", c, recurrence[c]);
	}
	printf("S: %i\n", soma);
}

_Bool getFreqTable(int fd, uint64_t *ptr){
	ssize_t size;
	unsigned char *buffer;

	buffer = malloc(BLOCK_SIZE);
	if(!buffer)
		return 0;

	memset(ptr, 0, 256 * sizeof(*ptr));

	while(1){
		size = read(fd, buffer, BLOCK_SIZE);

		if(size <= 0)
			break;

		//byte = 1; array[1]++; array[buffer[c]]
		for(int c = 0; c < (unsigned int)size; c++)
			ptr[buffer[c]]++;
	}
	free(buffer);

	return (size >= 0);
}

int main(){
	int fd = open("test", O_RDONLY); //ID
	if(!fd)
		return EXIT_FAILURE;

	//uint64_t ptr[256]; //[0, ... , 255] // 00000000, 00000001, 00000010, 00000011, 00000100 // ptr[0] = freq
	//comentei aqui para um teste Ass: Anderson lindo
	uint64_t *ptr = (uint64_t *)malloc(sizeof(uint64_t)*256);
	if(!ptr)
		return EXIT_FAILURE;
	
	_Bool status = getFreqTable(fd, ptr);
	if(!status)
		return EXIT_FAILURE;

	struct lists_s *list = NULL;
	list_create(&list);

	for(int i = 0; i < 256; i++){
		if(ptr[i] == 0)
			continue;

		struct data *data = (struct data *)malloc(sizeof(struct data));

		char *byte = malloc(1);
		*byte = i; //	[0]

		uint64_t *freq = malloc(sizeof(uint64_t));
		*freq = ptr[i]; //	[12664]

		data->data_1 = freq; //	[12664]
		data->data_2 = byte; // [0]

		enfilerar(list, data);
	}
	char **table;//dicionario

	printf("SORTED: \n");
	list_print(list);
	mount_tree(list);//montar a arvore
	
	free(ptr);
	close(fd);
	return 0;
}
