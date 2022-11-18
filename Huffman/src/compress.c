#include "../include/compress.h"
//...//
#include "../include/list.h"
#include "../include/tree.h"
#include "../include/helpful.h"

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


	printf("S: %lu\n", size);
	return (size >= 0);
}

// Função que cria a Fila de Prioridades:

_Bool create_queue(uint64_t *array_freq, LIST *list){
	for(int i = 0; i < 256; i++){
		if(array_freq[i] == 0)
			continue;

		DATA *data = (DATA *)malloc(sizeof(struct data));
		if(!data)
			return 0;

		unsigned char *byte = malloc(1);
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

		_Bool status = list_enqueue(list, tree);
		if(!status)
			return 0;
	}

	return 1;
}

// Função que escreve o cabeçalho do arquivo:

_Bool write_header(int compressed_file, int trash, int size_tree, unsigned char *tree){
	ssize_t size;
	unsigned char first_byte, second_byte;

	unsigned char byte_trash = (unsigned char)(trash << 5);
	unsigned char byte_size_tree = ((unsigned char)(size_tree >> 8));

	first_byte = byte_trash | byte_size_tree;
	second_byte = (unsigned char)size_tree;

	size = write(compressed_file, &first_byte, 1);
	if(size == -1)return 0;
	
	size = write(compressed_file, &second_byte, 1);
	if(size == -1)return 0;

	size = write(compressed_file, tree, size_tree);
	if(size == -1)return 0;

	return 1;
}

// Função que escreve o Bit-Stream:

_Bool write_bit_stream(int compressed_file, int fd, char **table){
	unsigned char *buffer;
	buffer = (unsigned char *)malloc(BLOCK_SIZE);
	if(!buffer)
		return 0;

	unsigned char byte = 0;
    int j = 0, bit_amount = 0, byte_pos = 7;

    while(1){
    	ssize_t size = read(fd, buffer, BLOCK_SIZE);
    	if(size <= 0){
    		break;
    	}

    	for(int c = 0; c < (unsigned int)size; c++){
    		j = 0;
    		char *new_path = table[buffer[c]];
        	while(new_path[j] != '\0'){
            	if(new_path[j] != '0'){
                	byte = set_bit(byte, byte_pos);
            	}
            
            	bit_amount++;
            	byte_pos--;
            	j++;

            	if(bit_amount >= 8){
                	if(write(compressed_file, &byte, 1) == -1){
                		return 0;
                	}

                	byte = 0;
                	byte_pos = 7;
                	bit_amount = 0;
            	}
        	}
    	}
    }

    if(bit_amount != 0){
        if(write(compressed_file, &byte, 1) == -1){
        	return 0;
        }
    }

	return 1;
}

// Função que escreve todos os dados no arquivo compactado:

_Bool write_in_file(int fd, int trash, int size_tree, char **table, uint64_t *array_freq, unsigned char *tree, int new_fd){
	_Bool status;

	status = write_header(new_fd, trash, size_tree, tree);
	if(!status)
		return 0;

	lseek(fd, 0, SEEK_SET);

	status = write_bit_stream(new_fd, fd, table);
	if(!status)
		return 0;

	return 1;
}

// Função principal que irá servir para compactar o arquivo:

_Bool compress(int fd, int new_fd){ // O 1º é o arquivo lido e o 2º é o aruquivo novo que receberá a compactação do 1º
	_Bool status;

	uint64_t *array_freq = (uint64_t *)malloc(sizeof(uint64_t)*256); // Aloca um array de ponteiros do tipo "uint_64" com 256 posições
	if(!array_freq){
		return 0;
	}
	
<<<<<<< HEAD
	status = get_freq_table(fd, array_freq); // Obtém a lista de frequência dos bits do arquivo lido
=======
	status = get_freq_table(fd, array_freq);
	printf("T: %i\n", status);
>>>>>>> 738b489 (.)
	if(!status){
		return 0;
	}

	LIST *list = list_create(); // Cria a lista que irá ser aplicado na fila de prioridade
	if(!list){
		return 0;
	}

	status = create_queue(array_freq, list); // Cria a fila de prioridade
	if(!status){
		return 0;
	}

	TREE *tree = mount_tree(list); // Montar a árvore
	if(!tree)
		return 0;

	int height = heightTree(tree); // Saber o tamanho da árvore

	char **table = NULL;

	table = allocTable(height + 1, table); // Alocar a matriz "dicionário"
	setTable(table, tree, "", height + 1); // Iniciar a matriz "dicionário" com cada os caminhos da árvore em cada linha

	int trash = 0;
	int size_tree = 0;

	trash = trash_size(table, array_freq); // Retorna a quantidade de lixo existente do fluxo de bits da árvore
	tree_size(tree, &size_tree); // Sobrescreve a variável "size_tree", que consequentemente é o tamanho da árvore

	unsigned char tree_str[8192]; // Cria o array de char que irá ser utilizado posteriormente

	int counter = 0;
	get_tree(tree, tree_str, size_tree, &counter); // Sobrescreve o array de char "tree_str" em pré-ordem baseado na árvore

	status = write_in_file(fd, trash, size_tree, table, array_freq, tree_str, new_fd); // Escreve um novo arquivo já compactado
	if(!status)
		return 0;

	free(array_freq); // Libera a variável após o uso dela, para não haver vazamento de memória
	return 1;
}