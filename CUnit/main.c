#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <CUnit/Basic.h>
#include "../include/list.h"
#include "../include/tree.h"

void test_list(){
    LIST *list = list_create();
}

void test_tree(){
    srand(time(NULL));
    TREE *tree = NULL;
    for(int i = 0; i < 10000; i++){
        DATA *data = (DATA *)malloc(sizeof(DATA));

        uint64_t *i = (uint64_t *)malloc(sizeof(uint64_t));
        *i = (rand() % 10000);
        printf("TESTE: %i")
        data->freq = i;
        if(insert_node(&tree, data))
            printf("Inserido!\n");
        else
            printf("Nao Inserido!\n");
    }
}

int main(){
    //test_list();
    test_tree();

    return 0;
}