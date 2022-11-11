#ifndef LISTA_H
#define LISTA_H

typedef struct lists_s LIST;
typedef struct node_s NODE;
typedef struct data DATA;

struct data{
	void *data_1; //freq
  	void *data_2; //byte
};

void error(char *msg, int code);
void list_create(struct lists_s **list);
void list_clear(struct lists_s *list);
void list_delete(struct lists_s **list);
void list_print(struct lists_s *list);
void list_insert(struct lists_s *list, char item);
int list_remove(struct lists_s *list, void *data);
void enfilerar(struct lists_s *list, void *data);
char get_item(struct lists_s *list);
DATA* top(LIST *list);

#endif