#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct lists_s LIST;
typedef struct node_s NODE;
typedef struct data DATA;

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

// Declaração das funções auxiliares:

LIST *list_create();
_Bool list_enqueue(struct lists_s *list, void *data);
_Bool list_delete(struct lists_s **list);
_Bool list_print(struct lists_s *list);
_Bool list_clear(struct lists_s *list);
void *list_dequeue(LIST *list);

#endif