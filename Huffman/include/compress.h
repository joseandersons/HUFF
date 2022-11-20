#ifndef COMPRESS_H
#define COMPRESS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
//--//
#include "list.h"

#define BLOCK_SIZE 8192 //8 KiB

_Bool get_freq_table(int fd, uint64_t *ptr);
_Bool mount_queue(uint64_t *array_freq, LIST *list);
_Bool compress(int fd, int new_fd);
_Bool write_bit_stream(int compressed_file, int fd, char **table);
_Bool write_header(int compressed_file, int trash, int size_tree, unsigned char *tree);
_Bool write_in_file(int fd, int trash, int size_tree, char **table, uint64_t *array_freq, unsigned char *tree, int new_fd);

#endif