#ifndef LISTA_H
#define LISTA_H

#include "tree.h"

typedef struct lists_s LIST;
typedef struct node_s NODE;
typedef struct data DATA;

#pragma pack(push, 1)

// Estrutura do Nó:

struct node_s{
    void *data;
    struct node_s *next;
};

// Estrutura da Lista:

struct lists_s{
    void *size;
    struct node_s *head, *tail;
};

// Estrutura dos Dados:

struct data{
	void *freq; //freq
  	void *byte; //byte
};

#pragma pack(pop)

// Declaração das funções auxiliares:

void *list_dequeue(LIST *list);
void error(char *msg, int code);
void list_create(struct lists_s **list);
void list_clear(struct lists_s *list);
void list_delete(struct lists_s **list);
void list_print(struct lists_s *list);
void list_insert(struct lists_s *list, char item);
int list_remove(struct lists_s *list, void *data);
void list_enqueue(struct lists_s *list, void *data);

#endif