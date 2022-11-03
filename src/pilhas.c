#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//..//
#include "../include/pilhas.h"

#pragma pack(push, 1)
struct node_s{
    int key;
    char item;
    struct node_s *next;
};

struct lists_s{
    struct node_s *head, *tail;
};

#pragma pop

void error(char *msg, int code){
    fputs(msg, stderr);
    exit(code);
}

void list_create(struct lists_s **list) {
    struct lists_s *aux;

    aux = (struct lists_s *)malloc(sizeof(struct lists_s));

    aux->head = (struct node_s *)malloc(sizeof(struct node_s));
    aux->tail = (struct node_s *)malloc(sizeof(struct node_s));
    aux->head->next = aux->tail;
    aux->tail->next = aux->tail;

    *list = aux;
}

void list_clear(struct lists_s *list){
    struct node_s *aux, *parent;
    
    if(!list){
        error("Error: list doesn't exist\n", -1);
    }
    
    parent = list->head;
    if(parent->next == list->tail){
        error("Error: empty list\n", -2);
    }
    
    aux = parent->next;
    while(aux != aux->next) {
        parent->next = aux->next;
        free(aux);
        aux = parent->next;
    }
}

void list_delete(struct lists_s **list){
    if(!list){
        error("Error: list doesn't exist\n", -1);
    }
    
    struct lists_s *aux;
    aux = *list;
    if(aux->head->next != aux->tail)
        list_clear(aux);
    
    free(aux->head);
    aux->head = NULL;
    
    free(aux->tail);
    aux->tail = NULL;
    
    free(aux);
    aux = NULL;
    
    *list = aux;
}

void list_print(struct lists_s *list){
    if(!list){
        error("Error: list doesn't exist\n", -1);
    }
    
    if(list->head->next == list->tail){
        error("Error: empty list\n", -2);
    }
    
    struct node_s *aux;

    aux = list->head->next;
    while(aux != aux->next){
        printf("%i -> ", aux->key);
        /*if(aux->next != aux->next->next)
            printf(" ");*/
        aux = aux->next;
    }
}

int list_remove(struct lists_s *list, int item){
    int resul = 0;

    struct node_s *parent = list->head;
    struct node_s *aux = parent->next;

    while(aux != aux->next){
        if(aux->item == item){
            resul = 1;
            break;
        }
        parent = parent->next;
        aux = aux->next;
    }

    parent->next = aux->next;

    if(aux != aux->next)
        free(aux);

    return resul;
}

void list_insert(struct lists_s *list, char item){
    struct node_s *new_node;
    new_node = (struct node_s *)malloc(sizeof(struct node_s));
    new_node->item = item;
    new_node->next = list->head->next;
    list->head->next = new_node;
}

void list_sorted_insert(struct lists_s *list, char item, int key){
    struct node_s *new_node, *aux, *parent;
    new_node = (struct node_s *)malloc(sizeof(struct node_s));

    parent = list->head;
    aux = parent->next;

    while(aux != aux->next){
        if(aux->key <= key)
            break;
        aux = aux->next;
        parent = parent->next;
    }

    new_node->item = item;
    new_node->key = key;

    new_node->next = parent->next;
    parent->next = new_node;
}

char get_item(struct lists_s *list){
    if(list->head->next == list->tail)
        return -1;

    NODE *aux = list->head->next;
    list->head->next = aux->next;
    char item = aux->item;
    free(aux);

    return item;
}
