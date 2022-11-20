#include "../include/decompress.h"
#include "../include/tree.h"
#include "../include/compress.h"

unsigned int bit_is_set(unsigned char byte, int i){
    unsigned char mask = (1 << i);
    return byte & mask;
}

// Função para saber o tamanho do lixo:

int size_for_trash(unsigned char byte){ // Recebe o byte que contém o lixo
    byte = byte >> 5;
    return (int)byte;
}

// Função para saber o tamanho da árvore que será criada:

int size_for_tree(unsigned char byte1, unsigned char byte2){ // 
    unsigned char mask = 31; // 
    return (int)(((byte1 & mask) << 8) | byte2);
}

// Função para escrever o arquivo já descompactado:

/*
    A função write_files() irá descomprimir os dados e, no final, escrever todos os bytes no arquivo descomprimido.
    Uma repetição será executada enquanto o índice i for menor que a quantidade de bytes do arquivo. Nesse laço, será
    verificado se o b 


*/

_Bool write_files(TREE *tree, int *i, unsigned char *bytes, int pos, int new_fd, int size_trash){
    unsigned char *buffer;
    TREE *aux = tree;
    int count = 0;

    buffer = (unsigned char *)malloc(BLOCK_SIZE);
	if(!buffer)return 0;

    while(*i < pos){

        for(int j = 7; j >= 0; j--){
            if(*i == pos-1 && j < size_trash)return 1;
            if(bit_is_set(bytes[*i], j))
                aux = aux->right;
            else
                aux = aux->left;
            
            if(!aux->left && !aux->right){
                unsigned char *byte = (unsigned char *)aux->data;
                buffer[count] = *byte;
                count++;
                aux = tree;
            }

            if(count >= BLOCK_SIZE){
                ssize_t status = write(new_fd, buffer, count);
                if(status == -1)return 0;
                count = 0;
            }
        }
        (*i)++;
    }

    return 1;
}

/*
    A função decompress irá direcionar a descompressão por meio das informações do cabeçalho do arquivo comprimido 
    (tamanho do lixo, tamanho da árvore e string correspondente a árvore de huffman).

    @param           fd          Arquivo comprimido
    @param           newfd       Arquivo onde serão escritos os dados descomprimidos
*/
_Bool decompress(int fd, int new_fd){

    unsigned char *bytes;

    if(fd == -1)return 0;
    if(new_fd == -1)return 0;

    lseek(fd, 0, SEEK_END);
    off_t pos = lseek(fd, 0, SEEK_CUR);

    lseek(fd, 0, SEEK_SET);
    bytes = malloc(pos);

    memset(bytes, 0, pos);

    ssize_t size = read(fd, bytes, pos);
    if(size == -1)return 0;
    int size_trash = size_for_trash(bytes[0]);
    int size_tree = size_for_tree(bytes[0], bytes[1]);

    int i = 2, size_max = size_tree + 2; 
    
    TREE *tree = mount_tres_for_decompress(NULL, &i, bytes, size_max);
    close(fd);

    write_files(tree, &i, bytes, pos, new_fd, size_trash);
    close(new_fd);

    return 1;
}