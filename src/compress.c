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

		DATA *data = (DATA *)malloc(sizeof(struct data));
		if(!data)
			return 0;

		char *byte = malloc(1);
		if(!byte)
			return 0;
		*byte = i;

		uint64_t *freq = (uint64_t *)malloc(sizeof(uint64_t));
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

void binary_print(int teste){
	int num = teste;
    unsigned char *bin = (unsigned char*)malloc(8);
    int aux;

    for (aux = 7; aux >= 0; aux--) {
        if (num % 2 == 0) {
            bin[aux] = '0';
            num = num / 2;
        }
        else {
            bin[aux] = '1';
            num = num / 2;
        }
    }

    printf("%s", bin);

    free(bin);
}

int set_bit(int c, int i){
	int mask = 1 << i;
	//binary_print(mask);
	return mask | c;
}

int is_bit_i_set(int c, int i){
	int mask = 1 << i;

	return mask & c;
}

uint16_t set_trash(int dest, int origin){
	uint16_t aux = dest;

	for(int i = 0; i < 3; i++){
		if(is_bit_i_set(origin, i)){
			aux = set_bit(aux, 13 + i);
		}
	}

	return aux;
}

uint16_t set_tree_size(int dest, int origin){
	uint16_t aux = dest;

	for(int i = 0; i < 13; i++){
		if(is_bit_i_set(origin, i)){
			aux = set_bit(aux, 0 + i);
		}
	}

	return aux;
}

_Bool write_header(int compressed_file, int trash, int size_tree, char *tree){
	unsigned char first_byte, second_byte;

	unsigned char byte_trash = (unsigned char)(trash << 5);
	unsigned char byte_size_tree = ((unsigned char)(size_tree >> 8));

	first_byte = byte_trash | byte_size_tree;
	second_byte = (unsigned char)size_tree;

	printf("Teste: ");
	binary_print(first_byte);
	binary_print(second_byte);
	printf("\n");

	write(compressed_file, &first_byte, 1);
	write(compressed_file, &second_byte, 1);
	write(compressed_file, tree, strlen(tree));

	return 1;
}

_Bool write_bit_stream(compressed_file, fd, char **table){
		/*
	while(1){
		size = read(fd, buffer, BLOCK_SIZE);
		if(size <= 0)
			break;
		
		for(int c = 0; c < (unsigned int)size; c++){
			write(compressed_file, table[buffer[c]], 1);
		}
	}
	*/
}

_Bool write_in_file(int fd, int trash, int size_tree, char **table, uint64_t *array_freq, char *tree, char *file_name){
	_Bool status;
	printf("T: ");
	binary_print(trash);
	printf("\n");
	printf("S: ");
	binary_print(size_tree);
	printf("\n");

	char *cpy, *state, *comp_file_name;

	cpy = strdup(file_name);
	if(!cpy)
		return 0;

	comp_file_name = strtok_r(cpy, ".", &state);
	comp_file_name = strcat(comp_file_name, ".huff");

	int compressed_file = open(comp_file_name, O_WRONLY | O_CREAT, 0764);
	if(compressed_file < 0)
		return 0;

	status = write_header(compressed_file, trash, size_tree, tree);
	if(!status)
		return 0;

	lseek(fd, 2 + strlen(tree), SEEK_SET);
	status = write_bit_stream(compressed_file, fd, table);
	if(!status)
		return 0;

	printf("TESTE: %s\n", comp_file_name);

	free(cpy);
	return 1;
}

_Bool compress(int fd, char *file_name){
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
	printf("\n-------\n");

	int height = heightTree(tree);

	char **table = NULL;

	table = allocTable(height + 1, table);
	setTable(table, tree, "", height + 1);

	int trash = 0;
	int size_tree = 0;

	trash = trash_size(table, array_freq);
	tree_size(tree, &size_tree);


	printf("\n");
	for(int c = 0; c < 256; c++){
		if(array_freq[c] != 0){
			printf("[%c] %s\n", c, table[c]);
		}
	}

	char *tree_str = (char *)malloc(size_tree + 1);
	// = get_tree(tree)

	status = write_in_file(fd, trash, size_tree, table, array_freq, tree_str, file_name);

	free(array_freq);
	return 1;
}