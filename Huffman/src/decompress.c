#include "../include/decompress.h"
#include "../include/tree.h"
#include "../include/compress.h"

/*
    A função bit_is_set() irá verificar se o bit na posição i está setado (retorna 1) ou não (retorna 0).

    @param          byte            Byte a ser analisado
    @param          i               Posição do bit
*/
unsigned int bit_is_set(unsigned char byte, int i){
    unsigned char mask = (1 << i);
    return byte & mask;
}

/*
    A função size_for_trash() recebe o primeiro byte do arquivo e realiza um shift bit de 5 para direita,
    retornando o tamanho do lixo.

    @param          byte            Primeiro byte do arquivo comprimido
*/

int size_for_trash(unsigned char byte){ // Recebe o byte que contém o lixo
    byte = byte >> 5;
    return (int)byte;
}

/*
    A função size_for_tree() retorna o tamanho da árvore e irá utilizar o número 31 em binário como mask para "zerar" os bits
    do lixo do primeiro byte e manter apenas os bits da árvore que estarão no primeiro byte. O "ou" com o segundo byte servirá
    para concatenar todos os bits.

    @param          byte1       Primeiro byte do arquivo comprimido
    @param          byte2       Segundo byte do arquivo comprimido

*/

int size_for_tree(unsigned char byte1, unsigned char byte2){ // 
    unsigned char mask = 31; // 
    return (int)(((byte1 & mask) << 8) | byte2);
}


/*
    A função write_files() irá descomprimir os dados e, no final, escrever todos os bytes no arquivo descomprimido.
    Uma repetição será executada até chegar no final do arquivo. Dentro desse laço, teremos outra repetição que visa
    percorrer a árvore de huffman por meio do caminho de cada byte. Na primeira condição, será verificado que, caso
    seja o último byte, o bit só deverá ser setado caso a posição deste seja menor do que o tamanho do lixo. Na segunda
    condição, caso o bit seja 1, navegaremos na árvore pela direita, caso contrário, navegaremos pela árvore pela esquerda.
    Essa verificação deve ser feita até se chegar em um nó folha, etapa em que será armazenado o byte no buffer. Quando o
    arquivo finalmente chegar no final, todos os bytes armazenados no buffer serão escritos diretamnete no novo arquivo
    descomprimido.

    @param          tree            Árvore de huffman
    @param          i               Índice para leitura dos bytes do arquivo
    @param          pos             Tamanho da string do arquivo
    @param          size_trash      Tamanho do lixo do último byte
    
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