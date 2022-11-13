#ifndef DECOMPRESS_H
#define DECOMPRESS_H

#include "../include/tree.h"

unsigned int bit_is_1(unsigned char byte, int i);
void decompress(TREE *root);

#endif