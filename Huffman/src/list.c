#include "../include/list.h"
//...//
#include "../include/tree.h"
#include "../include/helpful.h"

/*  

    A função list_create() serve somente para criar a lista encadeada e retorna a mesma no final, caso não consiga retorna NULL.

*/

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

/*  

    A função list_clear() serve para limpar todos os nós da lista encadeada e retorna 1 caso dê certo,
    para caso de retornar 0 é porque possivelmente ela já tenha chegado vazia.

    @param          list            Lista que será limpada

*/

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

/*  

    A função list_delete() serve para deletar a lista e retorna 1 caso dê certo, caso dê errado ela retorna 0. 
    A list_delete() necessita da list_clear() para que seja limpado todos os nós antes de dar o devido free() na lista.

    @param          list            Lista que será deletada

*/

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

/*  

    A função list_print() serve para imprimir a lista encadeada.
    Utiliza-se um auxiliar para assim não se alterar o "original" e perder a referência do mesmo.

    @param          list            Lista que será imprimida na tela

*/

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

/*  

    A função list_enqueue() serve para introduzir um nó na lista de forma crescente.
    Ele primeiro aloca o espaço de memória para o nó, caso dê errado ele já retorna "0". Cria-se um auxiliar para receber a
    cabeça da lista e não alterar acidentalmente algo da lista "original". Faz os devidos "castings" para os dados que serão 
    introduzidos e ao mesmo sendo utilizados por condicionais posteriormente. Dentro do laço ele percorre a lista até achar 
    um dado que seja menor ou igual ao dado que será introduzido, quando achado ele insere o nó na lista. Já com o nó introduzido, 
    incrementa-se o tamanho da lista, para controle de tamanho, e no final retorna "1" caso dê tudo certo.

    @param          list            Lista que será feita as alterações
    @param          data            Dado que será introduzido à lista

*/

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

/*  

    A função list_dequeue() serve para retirar o primeiro dado da lista.
    Pergunta-se antes se a lista está vazia, caso esteja retorna um dado NULL, caso não entre na condicional, ele cria
    auxiliares para salvar os dados que serão removidos, passa a cabeça da lista para o próximo nó e decrementa o tamanho da lista.
    No final ele faz o free() para a variável auxiliar, para não haver vazamento de memória, e retorna o dado que será removido.

    @param          list            Lista ao qual terá o dado removido

*/

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