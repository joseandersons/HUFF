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

// Função que cria a Fila de Prioridades:

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

// Função que escreve o cabeçalho do arquivo:

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

// Função que escreve o Bit-Stream:

_Bool write_bit_stream(int compressed_file, int fd, char **table)
{
    ssize_t size;
    unsigned char *buffer, *buffer_write;

    buffer = (unsigned char *)malloc(BLOCK_SIZE);
    if (!buffer)
        return 0;

    buffer_write = (unsigned char *)malloc(BLOCK_SIZE);
    if (!buffer_write)
        return 0;

    int byte = 0, bit_amount = 0, byte_pos = 7, buffer_count = 0;

    while (1)
    {
        size = read(fd, buffer, BLOCK_SIZE);
        if (size <= 0)
            break;

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
                    size = write(compressed_file, buffer_write, BLOCK_SIZE);
                    if (size == -1)
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

// Função que escreve todos os dados no arquivo compactado:

_Bool write_in_file(int fd, int trash, int size_tree, char **table, uint64_t *array_freq, unsigned char *tree, int new_fd)
{
    _Bool status;

    status = write_header(new_fd, trash, size_tree, tree); // Escreve o cabeçalho do arquivo
    if (!status)
        return 0;

    lseek(fd, 0, SEEK_SET);

    status = write_bit_stream(new_fd, fd, table); //
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
    if (!array_freq)
        return 0;

    status = get_freq_table(fd, array_freq); // Obtém a lista de frequência dos byts do arquivo lido
    if (!status)
        return 0;

    LIST *list = list_create();
    if (!list)
        return 0;

    status = mount_queue(array_freq, list);
    if (!status)
        return 0;

    TREE *tree = mount_tree(list); // Montar a árvore
    if (!tree)
        return 0;

    int height = heightTree(tree); // Saber o tamanho da árvore

    char **table = NULL;

    table = allocTable(height + 1, table); // Alocar a matriz "dicionário"
    setTable(table, tree, "", height + 1); // Iniciar a matriz "dicionário" com cada os caminhos da árvore em cada linha

    int trash = 0;
    int size_tree = 0;

    trash = trash_size(table, array_freq); // Retorna a quantidade de lixo existente do fluxo de bits da árvore
    tree_size(tree, &size_tree);           // Sobrescreve a variável "size_tree", que consequentemente é o tamanho da árvore

    int counter = 0;
    unsigned char tree_str[8192]; // Cria o array de char que irá ser utilizado posteriormente

    get_tree(tree, tree_str, size_tree, &counter); // Sobrescreve o array de char "tree_str" em pré-ordem baseado na árvore

    status = write_in_file(fd, trash, size_tree, table, array_freq, tree_str, new_fd); // Escreve um novo arquivo já compactado
    if (!status)
        return 0;

    free(array_freq); // Libera a variável após o uso dela, para não haver vazamento de memória
    return 1;
}