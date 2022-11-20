#include "../include/tree.h"

void error(char *msg, int code){
    fputs(msg, stderr);
    exit(code);
}

struct tree_s *tree_create(int item, struct tree_s *left, struct tree_s *right){
    struct tree_s *new_tree = (struct tree_s *)malloc(sizeof(struct tree_s));

    new_tree->item = item;
    new_tree->left = left;
    new_tree->right = right;

    return new_tree;
}

struct tree_s *tree_search(struct tree_s *bt, int item, int *count){
    (*count)++;
    if((bt == NULL) || (bt->item == item)){
        return bt;
    }else if(bt->item > item) {
        return tree_search(bt->left, item, count);
    }else{
        return tree_search(bt->right, item, count);
    }
}

struct tree_s *tree_add(struct tree_s *bt, int item){
    TREE *temp = tree_create(item, NULL, NULL);;
    if(bt == NULL){
        bt = temp;
        return bt;
    }
    
    TREE *current = bt;
    TREE *parent = NULL;
    while(1){
        parent = current;

        if(parent->item > item){
            current = current->left;
            
            if(current == NULL) {
               parent->left = temp;
               return bt;
            }
        }else{
            current = current->right;
            
            if(current == NULL){
               parent->right = temp;
               return bt;
            }
        }
    }

    return bt;
}

void print_pre_order(struct tree_s *tree){
    printf("(");
    if(tree != NULL){
        printf("%i", tree->item);
        print_pre_order(tree->left);
        print_pre_order(tree->right);
    }
    printf(")");
}