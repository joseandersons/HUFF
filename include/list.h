#ifndef LISTA_H
#define LISTA_H

#define ERROR "\033[1;38;5;1m"
#define WHITE_INTENSE "\033[1;38;5;15m"
#define WHITE "\033[1;38;5;15m"
#define YELLOW "\033[1;38;5;220m"
#define BLUE "\033[38;5;220m"
#define RESET "\033[0m"
#define REVEAL "\033[28m"
#define BLINK "\033[6;38;5;214m"
#define ITALIC "\033[3;2m"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

LIST *list_create();
_Bool list_enqueue(struct lists_s *list, void *data);
void *list_dequeue(LIST *list);
void error(char *msg, int code);
void list_clear(struct lists_s *list);
void list_delete(struct lists_s **list);
void list_print(struct lists_s *list);
void list_insert(struct lists_s *list, char item);

#endif