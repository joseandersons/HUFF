#ifndef COMPRESS_H
#define COMPRESS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

//--//
#include "list.h"

_Bool get_freq_table(int fd, uint64_t *ptr);
_Bool create_queue(uint64_t *array_freq, LIST *list);
_Bool compress(int fd, char *file_name);
_Bool write_bit_stream(int compressed_file, int fd, char **table);

#endif