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

// Obtém a frequência da tabela Hash:

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

		for(int c = 0; c < (unsigned int)size; c++)
			ptr[buffer[c]]++;
	}
	free(buffer);

	return (size >= 0);
}

int main(){
	int fd = open("arq", O_RDONLY); //ID
	if(fd == -1){
		error("Arquivo não encontrado/sem permissão", 1);
	}

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
		*byte = i;

		uint64_t *freq = malloc(sizeof(uint64_t));
		*freq = ptr[i];

		data->freq = freq;
		data->byte = byte;

		TREE *tree = create_tree(data, NULL, NULL);
		tree->data = data;

		enfilerar(list, tree);
	}

	printf("SORTED: \n");
	list_print(list);

	TREE *tree = mount_tree(list);//montar a arvore
	printf("TREE:\n");
	print_pre_order(tree);
	printf("\n");
	
	free(ptr);
	close(fd);
	return 0;
}
