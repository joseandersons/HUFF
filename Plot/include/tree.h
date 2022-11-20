#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//--//
typedef struct tree_s TREE;

struct tree_s{
    int item;
    int h;
    struct tree_s *left, *right;
};

TREE *tree_create(int item, TREE *left, TREE *right);
TREE *tree_search(TREE *bt, int item, int *count);
TREE *tree_add(TREE *bt, int item);
void print_pre_order(TREE *tree);

int is_balanced(TREE *bt);
int max(int a, int b);
int height(TREE *bt);
int balance_factor(TREE *bt);
int is_balanced(TREE *bt);
TREE *rotate_right(TREE *bt);
TREE *rotate_left(TREE *bt);
TREE* avl_add(TREE *bt, int item);

#endif