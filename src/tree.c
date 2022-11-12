#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

//..//
#include "../include/main_panel.h"
#include "../include/lista.h"

struct tree{
    void *data;
    struct tree *left, *right;
};

struct tree *create_tree(void *data, struct tree *left, struct tree *right){
    struct tree *new_tree;
    if((new_tree = (struct tree*)malloc(sizeof(struct tree))) == NULL)return NULL;
    
    new_tree->data = data;
    new_tree->left = left;
    new_tree->right = right;

    return new_tree;
}

void mount_tree(LIST *list){
    if(list->head->next){
        struct tree *leaf1, *leaf2;
        leaf1->data = top(list);
        leaf2->data = top(list);

        DATA *new_data, *data1, *data2;
        if((new_data = (DATA*)malloc(sizeof(DATA))) == NULL)getchar();
        data1 = (DATA*)leaf1->data;
        data2 = (DATA*)leaf2->data;

        uint64_t *frequency1 = (uint64_t*)data1->data_1;
        uint64_t *frequency2 = (uint64_t*)data2->data_1;

        uint64_t *new_frequecy = *frequency1 + *frequency2;
        unsigned char *new_byte = '*';

        new_data->data_1 = new_frequecy;
        new_data->data_2 = new_byte;

        struct tree *new_tree = create_tree(new_data, leaf1, leaf2);

        enfilerar(new_tree, new_data);
    }
    
}
int main(){
    
}