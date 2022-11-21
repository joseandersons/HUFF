#include "../include/tree.h"

int max(int a, int b){
    return (a > b) ? a : b;
}

int height(TREE *bt){
    if (bt == NULL) {
        return -1;
    }else{
        return 1 + max(height(bt->left), height(bt->right));
    }
}

int balance_factor(TREE *bt){
    if(bt == NULL){
        return 0;
    }else if((bt->left != NULL) && (bt->right != NULL)){
        return (bt->left->h - bt->right->h);
    }else if((bt->left != NULL) && (bt->right == NULL)){
        return (1 + bt->left->h);
    }else{
        return (-bt->right->h - 1);
    }
}

int is_balanced(TREE *bt){
    int bf = height(bt->left) - height(bt->right);
    return ((-1 <= bf) && (bf <= 1));
}

TREE *rotate_left(TREE *bt){
    TREE *sb_root = NULL;

    if(bt != NULL && bt->right != NULL){
        sb_root = bt->right;
        bt->right = sb_root->left;
        sb_root->left = bt;
    }

    sb_root->h = height(sb_root);
    bt->h = height(bt);

    return sb_root;
}

TREE *rotate_right(TREE *bt){
    TREE *sb_root = NULL;

    if(bt != NULL && bt->left != NULL){
        sb_root = bt->left;
        bt->left = sb_root->right;
        sb_root->right = bt;
    }

    sb_root->h = height(sb_root);
    bt->h = height(bt);

    return sb_root;
}

TREE *avl_add(TREE *bt, int item){
    if(bt == NULL) {
        return tree_create(item, NULL, NULL);
    }else if (bt->item > item) {
        bt->left = avl_add(bt->left, item);
    }else {
        bt->right = avl_add(bt->right, item);
    }

    bt->h = height(bt);
    TREE *child;

    if(balance_factor(bt) == 2 || balance_factor(bt) == -2) {
        if(balance_factor(bt) == 2){
            child = bt->left;
            if(balance_factor(child) == -1) {
                bt->left = rotate_left(child);
            }
            bt = rotate_right(bt);
        }else if(balance_factor(bt) == -2) {
            child = bt->right;
            if(balance_factor(child) == 1) {
                bt->right = rotate_right(child);
            }
            bt = rotate_left(bt);
        }
    }
    return bt;
}