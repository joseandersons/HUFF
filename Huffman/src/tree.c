#include "../include/tree.h"
#include "../include/list.h"
#include "../include/main_panel.h"

// Função que cria o nó e retorna o mesmo:

void print_pre_order(TREE *bt){
    if(bt){
        DATA *data = (DATA *)bt->data;
        unsigned char *byte = (unsigned char *)data->byte;

        printf("%c", *byte);
        print_pre_order(bt->left);
        print_pre_order(bt->right);
    }
}

_Bool insert_node(TREE **bt, void *data){
    if(!(*bt)){
        *bt = create_tree(data, NULL, NULL);
        return 1;
    }

    DATA *data_freq = (DATA *)data_freq                         ;
    DATA *data_bt = (DATA *)(*bt)->data;
    uint64_t *freq_bt = (uint64_t *)data_bt->freq;

    uint64_t *freq_data = (uint64_t *)data_freq->freq;

    if(*freq_data < *freq_bt){
        if(!(*bt)->left){
            (*bt)->left = create_tree(data, NULL, NULL);
            return 1;
        }
        insert_node(bt, data);
    }else if(*freq_data > *freq_bt){
        if(!(*bt)->right){
            (*bt)->right = create_tree(data, NULL, NULL);
            return 1;
        }
        insert_node(bt, data);
    }else{
        return 0;
    }

    return 1;
}

// Cria o no e retorna o no

struct tree *create_tree(void *data, struct tree *left, struct tree *right){
    struct tree *new_tree;
    if((new_tree = (struct tree*)malloc(sizeof(struct tree))) == NULL)return NULL;
    
    new_tree->data = data;
    new_tree->left = left;
    new_tree->right = right;

    return new_tree;
}

// Função para montar a árvore e retornar a mesma:

TREE *mount_tree(LIST *list){
    while(list->head->next != list->tail){
        struct tree *leaf1, *leaf2;
        DATA *new_data, *data1, *data2;

        leaf1 = (TREE *)list_dequeue(list);
        if(!leaf1)
            return NULL;

        leaf2 = (TREE *)list_dequeue(list);
        if(!leaf2)
            return NULL;

        new_data = (DATA*)malloc(sizeof(DATA));
        if(!new_data)
            return NULL;

        data1 = (DATA*)leaf1->data;
        data2 = (DATA*)leaf2->data;

        uint64_t *frequency1 = (uint64_t*)data1->freq;
        uint64_t *frequency2 = (uint64_t*)data2->freq;

        uint64_t *new_frequecy = (uint64_t*)malloc(sizeof(uint64_t));
        unsigned char *new_byte = (unsigned char *)malloc(sizeof(unsigned char));
        
        *new_frequecy = *frequency1 + *frequency2;
        *new_byte = '*';

        new_data->freq = new_frequecy;
        new_data->byte = new_byte;

        struct tree *new_tree = create_tree(new_data, leaf1, leaf2);

        _Bool status = list_enqueue(list, new_tree);
        if(!status)
            return NULL;

        uint64_t *size = (uint64_t *)list->size;

        if(*size == 1)
            break;
    }

    TREE *tree_huff = list_dequeue(list);
    if(!tree_huff)
        return NULL;

    return tree_huff;
}

// Função para calcular a altura da árvore:

int heightTree(struct tree *root){
    if(!root)
        return 0;

    int left, right;

    left = heightTree(root->left) + 1;
    right = heightTree(root->right) + 1;

    return left >= right ? left : right;
}

// Função que aloca uma nova tabela: 

char **allocTable(int height, char **table){
    
    int i = 0;

    table = malloc(sizeof(char *) * 256);
    if(!table)
        return NULL;

    for(i = 0; i < 256; i++){
        table[i] = calloc(height, sizeof(char)); // Percorre a matriz "dicionário" e inicia como 0 para cada linha dela
    }

    return table;
}

// Função que percorre a árvore salvando o caminho percorrido até chegar a um nó folha:

void setTable(char **table, struct tree *root, char *path, int height){
    if(root == NULL) return;

    char left[height], right[height];

    if(root->left == NULL && root->right == NULL){
        DATA *data = (DATA *)root->data;
        unsigned char *byte_ptr = (unsigned char *)data->byte;
        unsigned char byte = *byte_ptr;
        strcpy(table[(int)byte], path);

    } else {
        strcpy(left, path);
        strcpy(right, path);

        // Concatenando na string: adiciona 0 se for para a esquerda e 1 se for para a direita
        strcat(left, "0");
        strcat(right, "1");

        setTable(table, root->left, left, height);
        setTable(table, root->right, right, height);
    }
}

// Função que conta quanto de lixo tem na árvore:

int trash_size(char **table, uint64_t *frequency){
    uint64_t bits = 0;

    for(int i = 0; i < 256; i++){
        if(frequency[i] != 0){
            bits += frequency[i] * strlen(table[i]);
        }
    }

    if(bits % 8 == 0) return 0;
    else return (8 - (bits % 8));
}

// Função que conta todos os nós da árvore:

void tree_size(struct tree *root, int *size){ // Recebe a árvore e um inteiro ao qual chega como 0
    if(root == NULL) return;

    if(root->left == NULL && root->right == NULL){
        DATA *data = (DATA *)root->data;
        unsigned char *byte_ptr = (unsigned char *)data->byte;
        unsigned char byte = *byte_ptr;
        if(byte == '*' || byte == '\\') *size += 1;
    }

    *size += 1;
    tree_size(root->left, size);
    tree_size(root->right, size);
}

// Função que pega a árvore em pré-ordem e coloca ela em uma string:

void get_tree(TREE *root, unsigned char *str, int size_tree, int *counter){ // Recebe a árvore, uma string nula, o tamanho da árvore e um contador
    if(root){
        DATA *data = (DATA *)root->data;
        unsigned char *byte_ptr = (unsigned char*)data->byte;
        unsigned char byte = *byte_ptr;

        if(root->left == NULL && root->right == NULL) {
            if(byte == '*' || byte == '\\'){
                str[*counter] = '\\';
                (*counter)++;
            }
        }
        str[*counter] = byte;

        (*counter)++;
        get_tree(root->left, str, size_tree, counter);
        get_tree(root->right, str, size_tree, counter);
    }

    if(*counter == size_tree)
        str[*counter] = '\0';
}

// Função para criar um nó da árvore:

TREE *create_node(unsigned char element){ // 
    TREE *new_node = (TREE*)malloc(sizeof(TREE));
    new_node->data = malloc(sizeof(unsigned char));
    memcpy(new_node->data, &element, sizeof(unsigned char));
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

// Função para remontar a árvore:

TREE *mount_tres_for_decompress(TREE *tree, int *i, unsigned char *bytes, int size_max){

    if((*i) >= size_max)return tree;

    else{     
        if(bytes[*i] == '\\'){
            
            (*i)++;
            tree = create_node(bytes[*i]);
            (*i)++;
        }
        else{
            
            if(bytes[*i] == '*'){
            
                tree = create_node(bytes[*i]);
                (*i)++;
                tree->left = mount_tres_for_decompress(tree, i, bytes, size_max);
                tree->right = mount_tres_for_decompress(tree, i, bytes, size_max);
            }
            else{
               
                tree = create_node(bytes[*i]);
                (*i)++;
                return tree;
            }
        }
        return tree;
    }
}
