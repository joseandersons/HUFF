#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
//--//
#include "list.h"

//typedef struct lists_s LIST;
typedef struct tree TREE;

struct tree{
    void *data;
    struct tree *left, *right;
};

TREE *mount_tree(LIST *list);
TREE *create_tree(void *data, TREE *left, TREE *right);
int heightTree(TREE *root);
char** allocTable(int height, char **table);
void setTable(char **table, TREE *root, char *path, int height);
void print_pre_order(TREE *bt);
int trash_size(char **table, uint64_t *frequency);
void tree_size(TREE *root, int *size);


#endif