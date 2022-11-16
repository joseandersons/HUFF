#ifndef DECOMPRESS_H
#define DECOMPRESS_H

#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>


unsigned int bit_is_set(unsigned char byte, int i);
int size_for_trash(unsigned char byte);
int size_for_tree(unsigned char byte1, unsigned char byte2);
_Bool write_files(TREE *tree, int *i, unsigned char *bytes, int pos, int new_fd, int size_trash);
_Bool decompress(int fd, int new_fd);

#endif