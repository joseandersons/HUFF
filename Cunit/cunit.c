#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h> 
#include "../Huffman/include/list.h"
#include "../Huffman/include/tree.h"

DATA *adc_test_data(unsigned char c, uint64_t i){
    DATA *data = (DATA*)malloc(sizeof(DATA));

    unsigned char *a1 = malloc(sizeof(unsigned char));
    *a1 = c;
    uint64_t *a2 = malloc(sizeof(uint64_t));
    *a2 = i;
    data->byte = a1;
    data->freq = a2;

    return data;
}
void list_test(){
    LIST *list = list_create();
    
    DATA *da1 = adc_test_data('a', 1002);
    DATA *da2 = adc_test_data('b', 16);
    DATA *da3 = adc_test_data('c', 2000);
    DATA *da4 = adc_test_data('d', 0);
    DATA *da5 = adc_test_data('e', 12);
    DATA *da6 = adc_test_data('f', 2500);
    DATA *da7 = adc_test_data('g', 1000);

    TREE *tree1 = create_tree(da1, NULL, NULL);
    TREE *tree2 = create_tree(da2, NULL, NULL);
    TREE *tree3 = create_tree(da3, NULL, NULL);
    TREE *tree4 = create_tree(da4, NULL, NULL);
    TREE *tree5 = create_tree(da5, NULL, NULL);
    TREE *tree6 = create_tree(da6, NULL, NULL);
    TREE *tree7 = create_tree(da7, NULL, NULL);

    list_enqueue(list, tree1);
    list_enqueue(list, tree2);
    list_enqueue(list, tree3);
    list_enqueue(list, tree4);
    list_enqueue(list, tree5);
    list_enqueue(list, tree6);
    list_enqueue(list, tree7);

    TREE *t1 = (TREE*)list_dequeue(list);
    TREE *t2 = (TREE*)list_dequeue(list);
    TREE *t3 = (TREE*)list_dequeue(list);
    
    DATA *aux1 = t1->data,
         *aux2 = t2->data,
         *aux3 = t3->data;
   
    CU_ASSERT(*(int*)aux1->freq == 0);
    CU_ASSERT(*(int*)aux2->freq == 12);
    CU_ASSERT(*(int*)aux3->freq == 16);
}

void tree_test(){
    
    DATA *da1 = adc_test_data('a', 1002);
    
    TREE *tree = create_tree(da1, NULL, NULL);

    DATA *da2 = adc_test_data('b', 16);
    DATA *da3 = adc_test_data('c', 2000);
    DATA *da4 = adc_test_data('d', 0);
    DATA *da5 = adc_test_data('e', 12);
    DATA *da6 = adc_test_data('f', 2500);
    DATA *da7 = adc_test_data('g', 1000);

    tree->left = create_tree(da2, NULL, NULL);
    tree->right = create_tree(da3, NULL, NULL);
    tree->left->left = create_tree(da4, NULL, NULL);
    tree->left->right = create_tree(da5, NULL, NULL);
    tree->right->left = create_tree(da6, NULL, NULL);
    tree->right->right = create_tree(da7, NULL, NULL);

    DATA *aux1 = tree->right->right->data, 
         *aux2 = tree->right->left->data,
         *aux3 = tree->right->data;

    CU_ASSERT(*(char*)aux1->byte == 'g');
    CU_ASSERT(*(char*)aux2->byte == 'f');
    CU_ASSERT(*(char*)aux3->byte == 'c'); 

}

int main(){
    CU_initialize_registry();

    CU_pSuite suite_list = CU_add_suite("List test", 0, 0);
    CU_add_test(suite_list, "list_test", list_test);

    CU_pSuite suite_tree = CU_add_suite("Tree test", 0, 0);
    CU_add_test(suite_tree, "tree_test", tree_test);

    CU_basic_set_mode(CU_BRM_NORMAL);
    CU_basic_run_tests();

    CU_cleanup_registry();

    //tree_test();
    return 0;
}