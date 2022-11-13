#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

//..//
#include "../include/tree.h"

// Cria o nó e retorna o mesmo:

struct tree *create_tree(void *data, struct tree *left, struct tree *right){
    struct tree *new_tree;
    if((new_tree = (struct tree*)malloc(sizeof(struct tree))) == NULL)return NULL;
    
    new_tree->data = data;
    new_tree->left = left;
    new_tree->right = right;

    return new_tree;
}


// Função para montar a árvore e retornar a mesma:

TREE *mount_tree(struct lists_s *list){
    
    while(list->head->next != list->tail){
        struct tree *leaf1, *leaf2;
        leaf1 = (TREE *)top(list);
        leaf2 = (TREE *)top(list);

        DATA *new_data, *data1, *data2;
        if((new_data = (DATA*)malloc(sizeof(DATA))) == NULL)getchar();
        data1 = (DATA*)leaf1->data;
        data2 = (DATA*)leaf2->data;

        uint64_t *frequency1 = (uint64_t*)data1->freq;
        uint64_t *frequency2 = (uint64_t*)data2->freq;

        uint64_t *new_frequecy = (uint64_t*)malloc(sizeof(uint64_t));
        unsigned char *new_byte = (unsigned char *)malloc(sizeof(unsigned char));
        
        *new_frequecy = *frequency1 + *frequency2;
        *new_byte = '*';

        new_data->freq = new_frequecy;
        new_data->byte = new_byte;

        struct tree *new_tree = create_tree(new_data, leaf1, leaf2);

        enfilerar(list, new_tree);

        if(list->size)break;
    }
    TREE *tree_huff = top(list);
    return tree_huff;
}

// Função para calcular a altura da árvore:

int heightTree(struct tree *root){
    int left, right;

    if(root == NULL) return -1;

    else{
        left = heightTree(root->left) + 1;
        right = heightTree(root->right) + 1;

        if(left > right) return left;
        else return right;
    }
}

// Alocar uma nova tabela: 

char** allocTable(int height, char **table){//declarei o table no despos,,, se tiver errado, depois mudar
    
    int i = 0;

    table = malloc(sizeof(char *) * 256);

    for(i = 0; i < 256; i++){
        table[i] = calloc(height, sizeof(char));
    }

    return table;
}

// Percorre a árvore salvando o caminho percorrido até chegar a um nó folha:

void setTable(char **table, struct tree *root, char *path, int height){
    if(root == NULL) return;

    char left[height], right[height];

    if(root->left == NULL && root->right == NULL){
        DATA *data = (DATA *)root->data;
        unsigned char *byte_ptr = (unsigned char *)data->byte;
        unsigned char byte = *byte_ptr;
        strcpy(table[(int)byte],  path);
        // ??????
    }

    else{
        strcpy(left, path);
        strcpy(right, path);

        //concatenando na string: adiciona 0 se for pra esqurda e 1 se for pra direita
        strcat(left, "0");
        strcat(right, "1");

        setTable(table, root->left, left, height);
        setTable(table, root->right, right, height);
    }
}