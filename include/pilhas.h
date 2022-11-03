#ifndef PILHAS_H
#define PILHAS_H

typedef struct lists_s LIST;
typedef struct node_s NODE;

void error(char *msg, int code);
void list_create(struct lists_s **list);
void list_clear(struct lists_s *list);
void list_delete(struct lists_s **list);
void list_print(struct lists_s *list);
void list_insert(struct lists_s *list, char item);
void enfilerar(struct lists_s *list, char *item);
int list_remove(struct lists_s *list, int item);
void list_sorted_insert(struct lists_s *list, char item, int key);
char get_item(struct lists_s *list);

#endif