#include "../include/compress.h"
#include "../include/list.h"
#include "../include/tree.h"

#define BLOCK_SIZE 8192 //8 KiB

// Obtém a frequência da tabela Hash:

_Bool get_freq_table(int fd, uint64_t *array_freq){
	ssize_t size;
	unsigned char *buffer;

	buffer = malloc(BLOCK_SIZE);
	if(!buffer)
		return 0;

	memset(array_freq, 0, 256 * sizeof(*array_freq));

	while(1){
		size = read(fd, buffer, BLOCK_SIZE);

		if(size <= 0)
			break;

		for(int c = 0; c < (unsigned int)size; c++)
			array_freq[buffer[c]]++;
	}

	free(buffer);

	return (size >= 0);
}

_Bool create_queue(uint64_t *array_freq, LIST *list){
	for(int i = 0; i < 256; i++){
		if(array_freq[i] == 0)
			continue;

		DATA *data = (struct data *)malloc(sizeof(struct data));
		if(!data)
			return 0;

		char *byte = malloc(1);
		if(!byte)
			return 0;
		*byte = i;

		uint64_t *freq = malloc(sizeof(uint64_t));
		if(!freq)
			return 0;
		*freq = array_freq[i];

		data->freq = freq;
		data->byte = byte;

		TREE *tree = create_tree(data, NULL, NULL);
		if(!tree)
			return 0;
		tree->data = data;

		_Bool status = list_enqueue(list, tree);
		if(!status)
			return 0;
	}

	return 1;
}

_Bool compress(int fd){
	_Bool status;

	uint64_t *array_freq = (uint64_t *)malloc(sizeof(uint64_t)*256);
	if(!array_freq){
		return 0;
	}
	
	status = get_freq_table(fd, array_freq);
	if(!status){
		return 0;
	}

	LIST *list = list_create();;
	if(!list){
		return 0;
	}

	status = create_queue(array_freq, list);
	if(!status){
		return 0;
	}

	printf("SORTED: \n");
	list_print(list);

	TREE *tree = mount_tree(list);//montar a arvore
	if(!tree)
		return 0;

	printf("\nTREE:\n");
	print_pre_order(tree);
	
	free(array_freq);
	return 1;
}