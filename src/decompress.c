#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>


unsigned int bit_is_1(unsigned char byte, int i){
    unsigned char mask = (1 << i);
    return byte & mask;
}

void decompress(TREE *root){
    int fd = open("teste", O_RDONLY);
    int new_fd = open("teste2", O_WRONLY | O_CREAT, 0764);

    TREE *aux = root;
    unsigned char byte;

    if(fd){
        while(read(fd, &byte, sizeof(unsigned char), 1, fd)){
            for(int i = 7; i  >=0; i--){
                if(bit_is_1(byte, i))
                    aux = aux->right;
                else
                    aux = aux->left;
                
                if(!aux->left && !aux->right){
                    write(new_fd, (char *)aux->data, 1);
                    aux = root;
                }
            }
        }
        fclose(fd);
        fclose(new_fd);
    }
    else{
        puts("ERRO AO ABRI ARQUIVO!!");
    }
}