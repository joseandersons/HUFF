#include "../include/list.h"
//...//
#include "../include/tree.h"
#include "../include/helpful.h"

// Função que cria uma lista dinâmica a partir da estrutura da Fila:

LIST *list_create(){
    LIST *new_list;

    new_list = (LIST *)malloc(sizeof(LIST));
    if(!new_list){
        return NULL;
    }

    new_list->head = (NODE *)malloc(sizeof(NODE));
    if(!new_list->head){
        return NULL;
    }

    new_list->tail = (NODE *)malloc(sizeof(NODE));
    if(!new_list->tail){
        return NULL;
    }

    new_list->head->next = new_list->tail;
    new_list->tail->next = new_list->tail;

    uint64_t *size = malloc(sizeof(uint64_t));
    if(!size){
        return NULL;
    }

    *size = 0;
    new_list->size = size;

    return new_list;
}

// Função que limpa todos os nós da fila dinâmica:

_Bool list_clear(LIST *list){
    NODE *aux, *parent;
    
    if(!list){
        error("Error: list doesn't exist\n");
        return 0;
    }
    
    parent = list->head;
    if(parent->next == list->tail){
        error("Error: empty list\n");
        return 0;
    }
    
    aux = parent->next;
    while(aux != aux->next) {
        parent->next = aux->next;
        free(aux);
        aux = parent->next;
    }

    return 1;
}

// Função que exclui a lista dinâmica:

_Bool list_delete(LIST **list){
    if(!list){
        error("Error: list doesn't exist\n");
        return 0;
    }
    
    LIST *aux;
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

    return 1;
}

// Função que imprime a lista dinâmica da estrutura Fila:

_Bool list_print(LIST *list){
    if(!list){
        error("Error: list doesn't exist\n");
        return 0;
    }
    
    if(list->head->next == list->tail){
        error("Error: empty list\n");
        return 0;
    }
    
    NODE *aux;
    aux = list->head->next;

    while(aux != aux->next){
        TREE *tree = (TREE *)aux->data;
        DATA *data = (DATA *)tree->data;

        uint64_t *freq = (uint64_t *)data->freq;
        unsigned char *byte = (unsigned char  *)data->byte;

        printf("[%x]\t->\t[%ld]\n", *byte, *freq);

        aux = aux->next;
    }

    return 1;
}

// Função que insere na lista dinâmica de forma crescente:

_Bool list_enqueue(LIST *list, void *data){
    NODE *new_node, *aux, *parent;

    uint64_t *size = (uint64_t*)list->size;
    new_node = (NODE *)malloc(sizeof(NODE));
    if(!new_node)
        return 0;

    parent = list->head;
    aux = parent->next;

    TREE *tree = (TREE *)data;
    DATA *data_freq = (DATA*)tree->data;

    uint64_t *freq_1 = (uint64_t *)data_freq->freq;

    while(aux != aux->next){
        DATA *data_aux;
        TREE *tree_aux = (TREE *)aux->data;

        data_aux = (DATA *)tree_aux->data;
        uint64_t *freq_2 = (uint64_t *)data_aux->freq;

        if(*freq_2 >= *freq_1)
            break;

        aux = aux->next;
        parent = parent->next;
    }
    new_node->data = tree;

    new_node->next = parent->next;
    parent->next = new_node;

    (*size)++;
    list->size = size;

    return 1;
}

// Função que salva a estrutura DATA em uma variável, passa para o próximo nó e remove ele, e depois retorna a variável:

void *list_dequeue(LIST *list){
    if(list->head->next == list->tail){
        return NULL;
    }

    NODE *remove = list->head->next;
    DATA *data = remove->data;

    NODE *aux = remove->next;
    list->head->next = aux;

    uint64_t *size = (uint64_t*)list->size;
    (*size)--;
    list->size = size;

    free(remove);
    return data;
}