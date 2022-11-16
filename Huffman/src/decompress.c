#include "../include/decompress.h"
#include "../include/tree.h"

unsigned int bit_is_set(unsigned char byte, int i){
    unsigned char mask = (1 << i);
    return byte & mask;
}

int size_for_trash(unsigned char byte){
    byte = byte >> 5;
    return (int)byte;
}

int size_for_tree(unsigned char byte1, unsigned char byte2){
    unsigned char mask = 31;
    return (int)(((byte1 & mask) << 8) | byte2);
}

_Bool write_files(TREE *tree, int *i, unsigned char *bytes, int pos, int new_fd, int size_trash){
    TREE *aux = tree;

    while(*i < pos){

        for(int j = 7; j  >= 0; j--){
            if(*i == pos-1 && j < size_trash)return 1;
            if(bit_is_set(bytes[*i], j))
                aux = aux->right;
            else
                aux = aux->left;
            
            if(!aux->left && !aux->right){
                unsigned char *byte = (unsigned char *)aux->data;
                int status = write(new_fd, byte, 1);
                if(status == -1)return 0;
                aux = tree;
            }
        }
        (*i)++;
    }
    return 1;
}

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

    int i = 2, size_max = size_tree + 2; // byte 0 é tam da thash e 1 é tam da tree
    
    TREE *tree = mount_tres_for_decompress(NULL, &i, bytes, size_max);
    close(fd);

    write_files(tree, &i, bytes, pos, new_fd, size_trash);
    close(new_fd);

    return 1;
}