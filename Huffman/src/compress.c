#include "../include/compress.h"
//...//
#include "../include/list.h"
#include "../include/tree.h"
#include "../include/helpful.h"

/*
    A função get_freq_table() pega as frequencias de cada byte lido do arquivo e coloca em um array e retorna true
    ou false de acordo com a execução do codigo dê certo.

    @param          fd          Arquivo original.
    @param       array_freq     Array onde seram armazenados as frequencias de cada byte, de acordo com sua posição
    dada no array de 256 posições.
*/

_Bool get_freq_table(int fd, uint64_t *array_freq)
{
    ssize_t size;
    unsigned char *buffer;

    buffer = malloc(BLOCK_SIZE);
    if (!buffer)
        return 0;

    memset(array_freq, 0, 256 * sizeof(*array_freq));

    while (1)
    {
        size = read(fd, buffer, BLOCK_SIZE);

        if (size <= 0)
            break;

        for (int c = 0; c < (unsigned int)size; c++)
            array_freq[buffer[c]]++;
    }

    free(buffer);

    return (size >= 0);
}

/*
    A função mount_queue() monta a fila de prioridade de acordo com cada frenquecia do array de frenquecia de forma crecente.
    A mesma percorre todo o array de frequencia de tamanho 256.
    Quando aquela posição do array tiver tamanho(frequência) 0 ele não é adicionado da lista e é pulado para a proxima posição, pois 
    aquele byte não está sendo utilizado.
    É feita a alocação de ponteiro *data do tipo DATA para que seja recebida a frequencia e o byte daquela posição e com esse ponteiro
    é feito a criação da tree.
    O byte é o proprio indice e a frequcia é o proprio tamanho(frequência) da posição do array de frequência.
    Depois de criada a árvore é feita o enfileramento daquele node do tipo árvore de forma crescente.
    Por fim retorna true ou false de acordo com a execução da função.

    @param          *array_freq         Array com a frequencia de cada byte.
    @param             *list            Fila de prioridade onde será enfilerado cada nó do tipo arvore.


*/

_Bool mount_queue(uint64_t *array_freq, LIST *list)
{
    for (int i = 0; i < 256; i++)
    {
        if (array_freq[i] == 0)
            continue;

        DATA *data = (DATA *)malloc(sizeof(struct data));
        if (!data)
            return 0;

        unsigned char *byte = malloc(1);
        if (!byte)
            return 0;

        uint64_t *freq = (uint64_t *)malloc(sizeof(uint64_t));
        if (!freq)
            return 0;

        *byte = i;
        *freq = array_freq[i];

        data->freq = freq;
        data->byte = byte;

        TREE *tree = create_tree(data, NULL, NULL);
        if (!tree)
            return 0;

        _Bool status = list_enqueue(list, tree);
        if (!status)
            return 0;
    }

    return 1;
}

/*
    A função write_header() Escreve no arquivo comprimido o tamanho do lixo, tamanho da arv e a propria arvora em pre-ordem e retorna true
    ou false de acordo com a execução da função.
    Define o primeiro byte do lixo com um shift bit para a esquerda 5 vezes e faz o | com tamanho da arvore feito o shift bit para a direira 8 vezes.
    Define o segundo byte do arquivo como o proprio tamanho da arv.
    Define o terceiro byte como a propria arv em pre-ordem.

    @param          compressed_file         Id do arquivo a ser comprimido.
    @param               trash              Tamanho do lixo.
    @param             size_tree            Tamanho da arv.
    @param               tree               Arvore em pre ordem.

*/

_Bool write_header(int compressed_file, int trash, int size_tree, unsigned char *tree)
{
    ssize_t size;
    unsigned char first_byte, second_byte;

    unsigned char byte_trash = (unsigned char)(trash << 5);
    unsigned char byte_size_tree = ((unsigned char)(size_tree >> 8));

    first_byte = byte_trash | byte_size_tree;
    second_byte = (unsigned char)size_tree;

    size = write(compressed_file, &first_byte, 1);
    if (size == -1)
        return 0;

    size = write(compressed_file, &second_byte, 1);
    if (size == -1)
        return 0;

    size = write(compressed_file, tree, size_tree);
    if (size == -1)
        return 0;

    return 1;
}

/*
    A função write_bit_stream(), escreve o bit-stream e retorna true ou false de acordo com a execução da função.
    
*/

_Bool write_bit_stream(int compressed_file, int fd, char **table)
{
    ssize_t size;
    unsigned char *buffer, *buffer_write;

    buffer = (unsigned char *)malloc(BLOCK_SIZE);
    if (!buffer) return 0;

    buffer_write = (unsigned char *)malloc(BLOCK_SIZE);
    if (!buffer_write) return 0;

    int byte = 0, bit_amount = 0, byte_pos = 7, buffer_count = 0;

    while(1){
        size = read(fd, buffer, BLOCK_SIZE);
        if (size <= 0) break;

        for (int c = 0; c < (unsigned int)size; c++)
        {
            char *new_path = table[buffer[c]];

            for (int j = 0; new_path[j] != '\0'; j++)
            {
                if (new_path[j] != '0')
                {
                    byte = set_bit(byte, byte_pos);
                }

                bit_amount++;
                byte_pos--;

                if (bit_amount >= 8)
                {
                    buffer_write[buffer_count] = byte;
                    buffer_count++;

                    byte = 0;
                    byte_pos = 7;
                    bit_amount = 0;

                }

                if (buffer_count >= BLOCK_SIZE)
                {
                    ssize_t size_write = write(compressed_file, buffer_write, BLOCK_SIZE);
                    if (size_write == -1)
                        return 0;

                    buffer_count = 0;
                }
            }
        }
    }

    if (bit_amount != 0)
    {
        buffer_write[buffer_count] = byte;
        buffer_count++;
    }

    if (buffer_count != 0)
    {
        size = write(compressed_file, buffer_write, buffer_count);
        if (size == -1)
            return 0;
    }

    free(buffer);
    free(buffer_write);
    return 1;
}

/*
    A função write_in_file() escreve as escreve o cabeçalho no novo arquivo.
    Primeiro escreve no arquivo compactado o lixo, tamanho da arv e a arvore e 
    depois escreve os bit_stream.

    @param          fd          Arquivo original.
    @param         trash        Tamanho do lixo.
    @param       size_tree      Tamanho da arv.
    @param         table        Dicionario.
    @param       array_freq     Array com as frequências dos bytes.
    @param         tree         Arvore montada.
    @param        new_fd        Arquivo a ser compactado. 


*/

_Bool write_in_file(int fd, int trash, int size_tree, char **table, uint64_t *array_freq, unsigned char *tree, int new_fd)
{
    _Bool status;

    status = write_header(new_fd, trash, size_tree, tree); 
    if (!status)
        return 0;

    lseek(fd, 0, SEEK_SET);

    status = write_bit_stream(new_fd, fd, table); 
    if (!status)
        return 0;

    return 1;
}

/*
    Recebe o arquivo original e comprime criando um novo arquivo.

    @param          fd          id do arquivo original.
    @param        new_fd        id do aquivo a ser comprimido.

*/

_Bool compress(int fd, int new_fd)
{
    _Bool status;

    uint64_t *array_freq = (uint64_t *)malloc(sizeof(uint64_t) * 256);
    if (!array_freq) return 0;

    status = get_freq_table(fd, array_freq); // Obtém a lista de frequência dos byts do arquivo lido
    if (!status) return 0;

    LIST *list = list_create();
    if (!list) return 0;

    status = mount_queue(array_freq, list);
    if (!status) return 0;

    TREE *tree = mount_tree(list);
    if (!tree) return 0;

    int height = heightTree(tree); // Altura da arvore

    char **table = NULL;

    table = allocTable(height + 1, table);//aloca e limpa a table
    setTable(table, tree, "", height + 1);

    int trash = 0;
    int size_tree = 0;

    trash = trash_size(table, array_freq);
    tree_size(tree, &size_tree);
    
    int counter = 0;
    unsigned char tree_str[8192];

    get_tree(tree, tree_str, size_tree, &counter);

    status = write_in_file(fd, trash, size_tree, table, array_freq, tree_str, new_fd);
    if (!status) return 0;

    free(array_freq);
    return 1;
}