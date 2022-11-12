#include "../include/main_panel.h"
#include "../include/lista.h"

typedef struct tree TREE;

struct tree{
    void *data;
    struct tree *left, *right;
};

struct tree *create_tree(void *data, TREE *left, TREE *right);
void mount_tree(LIST *list);
int heightTree(struct tree *root);
char** allocTable(int height, char **table);
void setTable(char **table, struct tree *root, char *path, int height);