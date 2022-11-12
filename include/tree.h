#ifndef TREE_H
#define TREE_H

#include "main_panel.h"
#include "lista.h"

typedef struct lists_s LIST;
typedef struct tree TREE;

struct tree{
    void *data;
    struct tree *left, *right;
};

struct tree *create_tree(void *data, TREE *left, TREE *right);
void mount_tree(struct lists_s *list);
int heightTree(struct tree *root);
char** allocTable(int height, char **table);
void setTable(char **table, struct tree *root, char *path, int height);

#endif