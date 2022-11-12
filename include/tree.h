#include "../include/main_panel.h"
#include "../include/lista.h"

typedef struct tree TREE;
struct tree *create_tree(DATA *data, TREE *left, TREE *right);
void mount_tree(LIST *list);