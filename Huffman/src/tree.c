#include "../include/tree.h"
#include "../include/list.h"
#include "../include/main_panel.h"

/*  

    A função print_pre_order() serve para printar a árvore em pré-ordem.
    Primeiro pergunta se a árvore é nula, caso seja ele apenas se encerra. Caso não seja nulo
    ele converte o "data" para uma string "unsigned char" e em seguida printa o caractere da
    string, logo após ele chama recursivamente para a esquerda e para a direita. A função se
    encerra quando a pilha de recursão se acabar.

    @param          bt          Árvore que será printada em pré-ordem

*/

void print_pre_order(TREE *bt){
    if(bt){
        DATA *data = (DATA *)bt->data;
        unsigned char *byte = (unsigned char *)data->byte;

        printf("%c", *byte);
        print_pre_order(bt->left);
        print_pre_order(bt->right);
    }
}

/*  

    A função create_tree() serve para criar a árvore e retornar a mesma no final.
    Primeiro se cria uma estrutura do tipo "TREE" e em seguida aloca memória para a mesma, caso
    não consiga alocar memória retorna-se NULL. Caso consiga alocar a memória ele coloca o dado
    no "data" dessa nova árvore, coloca seu ponteiro da esquerda e a da sua direita, no final ele
    retorna a árvore criada.

    @param          data            Dado que será colocado no "data" da nova árvore
    @param          left            Ponteiro que indica a esquerda da árvore
    @param          right           Ponteiro que indica a direita da árvore

*/

struct tree *create_tree(void *data, struct tree *left, struct tree *right){
    struct tree *new_tree;
    if((new_tree = (struct tree*)malloc(sizeof(struct tree))) == NULL)return NULL;
    
    new_tree->data = data;
    new_tree->left = left;
    new_tree->right = right;

    return new_tree;
}

/*  

    A função mount_tree() serve para montar a árvore para a descompactação.
    Enquanto a cabeça da lista for diferente da cauda: ele criará 5 estruturas, 2 do tipo "TREE" e as outras 3 do tipo
    "DATA", em seguida ele faz o list_dequeue() e armazena numa estrutura criada do tipo "TREE" e em seguida pergunta 
    se ele recebeu um elemento, caso não receba ele retorna NULL, esse procedimento é feito 2 vezes. Em seguida ele aloca
    memória para uma das estruturas do tipo "DATA", caso não consiga alocar ele retorna NULL. Logo após pegamos o "data"
    das estruturas "TREE" que fizemos os list_dequeue() e armazenamos nas outras 2 estruturas restantes do tipo "DATA",
    também pegamos a frequência e armazenamos em outras 2 variáveis do tipo "uint64_t". Criamos e alocamos memória para
    outras 2 variáveis ponteiros, uma "uint64_t" e outra "unsigned char". Na variável de inteiro nós somamos as frequências
    dos nós que fizemos o dequeue, na variável char nós colocamos o "*" para indicar que é um nó pai. Armazenamos a soma das
    frequências e o novo char respectivamente nas variáveis da estrutura "DATA" restante e depois criamos uma "nova" árvore
    com os dados inseridos. Em seguida enfileiramos essa "nova" árvore na fila novamente e perguntamos se conseguiu enfileirar,
    caso não consiga retorna NULL. No final nós perguntamos se o tamanho da fila é 1, caso seja nós paramos o laço de repetição.
    Por último nós fazemos o dequeue do último elemento da fila, caso dê errado nós retornamos NULL, caso dê certo nós apenas
    retornamos a árvore que criamos com o processo.

    @param          list            Lista que contém todos os nós da fila

*/

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

/*  

    A função heightTree() serve para sabermos a altura da árvore.
    Primeiro se pergunta se a árvore é nula, caso seja retorna 0 (já que é uma função do tipo int). Caso não
    seja nula, criamos duas variáveis do tipo int, em seguida nós atribuímos a cada uma dessas variáveis uma chamada
    recursiva, uma chamada para a esquerda e outra para a direita. No final retornamos um operador ternário, caso uma
    das variáveis seja maior ou igual a outra nós retornaremos a maior delas.

    @param          root            Árvore que será verificada a altura

*/

int heightTree(struct tree *root){
    if(!root)
        return 0;

    int left, right;

    left = heightTree(root->left) + 1;
    right = heightTree(root->right) + 1;

    return left >= right ? left : right;
}

/*  

    A função allocTable() serve para alocarmos uma matriz com todos os valores como 0.

    @param          height          A altura da árvore + 1 para criarmos com uma linha a mais  
    @param          table           Matriz que será alocada

*/

char **allocTable(int height, char **table){
    
    int i = 0;

    table = malloc(sizeof(char *) * 256);
    if(!table)
        return NULL;

    for(i = 0; i < 256; i++){
        table[i] = calloc(height, sizeof(char));
    }

    return table;
}

/*  

    A função setTable() serve para salvarmos o caminho percorrido para alcançar cada byte posteriormente.
    Primeiro se pergunta se a árvore é nula, caso seja nós encerramos a função. Caso não seja nula nós criamos 2 vetores
    de char com o tamanho sendo o "height", porque esse será o tamanho máximo dessa string. Em seguida nós perguntamos se
    é uma folha, caso seja nós criamos uma estrutura do tipo "DATA" e atribuímos o data do "root" para ele, em seguida nós
    transformamos ele para um caractere e copiamos o path para a tabela na linha do caractere transformada em inteiro. Caso
    não seja uma folha nós copiamos o path para os 2 vetores que criamos, em seguida para o vetor que indica a esquerda nós
    concatenamos o "0", para a direita nós colocamos o "1". Por último nós chamamos recursivamente para a esquerda passando o
    path como o vetor left e para a direita passando como o right, ammbos já com suas devidas alterações.
 
    @param          table           Matriz que será alocada
    @param          root            Árvore que será percorrida
    @param          path            String que salvará o caminho percorrido
    @param          height          Altura da árvore + 1, já contando com a linha a mais da matriz

*/

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

        strcat(left, "0");
        strcat(right, "1");

        setTable(table, root->left, left, height);
        setTable(table, root->right, right, height);
    }
}

/*  

    A função trash_size() serve para sabermos se há lixo ou não no último byte do arquivo.
    Criamos uma variável do tipo "uint64_t" que se inicia como 0, logo em seguida criamos um laço de repetição
    que se executa 256 vezes e nele se pergunta se o índice do laço naquela posição do vetor de frequência é
    diferente de 0, se for quer dizer que há a presença de pelo menos um byte naquela posição. Caso entre na
    condicional nós somamos a variável inteira "bits" com a frequência do byte vezes o tamanho do caminho na
    tabela de caminhos que criamos mais a própria variável inteira, fazemos isso até acabar o laço de repetição.
    Quando acabar a repetição nós perguntamos se a variável "bits" é divisível por 8, caso seja nós retornamos 0 
    porque não há lixo por conta de serem bytes completos. Se não for divisível por 8 nós retornaremos a variável
    "bits" com resto 8 menos o próprio 8, 8 é justamente o tamanho de um byte, se for menos que isso há o risco de
    o retorno ser 0.
 
    @param          table           Matriz que contém os novos caminhos dos bytes
    @param          frequency       Vetor que contém as frequências dos bytes

*/

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

/*  

    A função tree_size() serve para sabermos qual o tamanho total da árvore contando todos os nós.
    Primeiro se pergunta se a árvore é nula, se for apenas se encerra a função. Caso a árvore não seja nula
    nós perguntamos se é uma folha, se for uma folha nós criamos uma estrutura do tipo "DATA" que irá pegar
    data da árvore, em seguida nós o transformamos em um caractere e perguntamos se o caractere é um "*" ou
    "/", se for nós incrementamos mais um no ponteiro acumulador de tamanho. Caso não seja uma folha nós
    incrementamos mais um no acumulador e chamamos recursivamente a árvore para a esquerda e para a direita.

    @param          root            Árvore que será percorrida
    @param          size            Variável que acumulará o tamanho da árvore

*/

void tree_size(struct tree *root, int *size){
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

/*  

    A função get_tree() serve para criarmos uma string que será usada na criação da árvore.
    Primeiro se pergunta se a árvore é nula, se for ele faz uma pergunta relaciondada a condição de parada,
    se não for condição de parada ele apenas se encerra, já que é uma função void. Caso a árvore não seja nula
    convertemos o data para ele virar um "unsigned char" (caractere sem sinal), em seguida perguntamos se é uma 
    folha, caso seja nós acrescentamos o caractere "/" na string e em seguida incrementamos no índice, fazemos
    isso para posteriormente na leitura da string sabermos quando é um nó filho ou não. Em seguida introduzimos
    o caractere na string, incrementamos no índice chamamos recursivamente tanto para a esquerda quanto para a
    direita da árvore. A condição de parada, como dito anteriormente, será quando o índice se igualar ao tamanho
    máximo da árvore, caso entre na condição de parada adicionamos o "/0" para indicar que é um final de string.

    @param          root            Árvore que será usada para ler-se os dados
    @param          str             String que será escrita para a descompactação
    @param          size_tree       Tamanho máximo da árvore
    @param          counter         Índice que irá ser usado para acessar o array e também como forma de parada

*/

void get_tree(TREE *root, unsigned char *str, int size_tree, int *counter){
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

/*  

    A função create_node() serve para criar o nó já com o dado inserido.
    Ela aloca memória tanto para o novo nó quanto para o dado que será inserido, em seguida ele copia a variável
    do dado para a variável "data" do novo nó criado, no final ele retorna esse novo nó criado já com o dado inserido.

    @param          element         Dado que será inserido

*/

TREE *create_node(unsigned char element){ // 
    TREE *new_node = (TREE*)malloc(sizeof(TREE));
    new_node->data = malloc(sizeof(unsigned char));
    memcpy(new_node->data, &element, sizeof(unsigned char));
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}

/*  

    A função mount_tres_for_decompress() serve principalemnte para recriar a árvore para a descompactação.
    Primeiro se pergunta se o índice é maior ou igual ao tamanho máximo da árvore, se sim ela já retorna a árvore,
    se não continua-se o código. Pergunta-se se o array "bytes" naquele índice é um "/", se for apenas ignoramos ele
    e incrementamos o índice, em seguida criamos um novo nó e incrementamos novamente o índice. Caso não seja um "/",
    perguntamos se é um "*" (nó pai), se for nós criamos um novo nó, incrementamos o índice e chamamos recursivamente 
    para a esquerda e para a direita. Caso não seja um "*" significa que é um nó filho, portanto, criamos um novo nó,
    incrementamos o índice e retornamos a árvore. Caso não seja um "/" ou um "*" ou um nó filho nós apenas retornamos
    a árvore.

    @param          tree                Estrutura da árvore que será remontada
    @param          i                   Índice que irá ser usado para acessar o array e também como forma de parada
    @param          bytes               Array que irá servir para recriar a árvore
    @param          size_max            Tamanho máximo da árvore

*/

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
