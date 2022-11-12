#ifndef LISTA_H
#define LISTA_H

#include "tree.h"

typedef struct lists_s LIST;
typedef struct node_s NODE;
typedef struct data DATA;

#pragma pack(push, 1)

struct node_s{
    void *data;
    struct node_s *next;
};

struct lists_s{
    void *size;
    struct node_s *head, *tail;
};

struct data{
	void *data_1; //freq
  	void *data_2; //byte
};

#pragma pack(pop)


void *top(LIST *list);
void error(char *msg, int code);
void list_create(struct lists_s **list);
void list_clear(struct lists_s *list);
void list_delete(struct lists_s **list);
void list_print(struct lists_s *list);
void list_insert(struct lists_s *list, char item);
int list_remove(struct lists_s *list, void *data);
void enfilerar(struct lists_s *list, void *data);
char get_item(struct lists_s *list);

#endif