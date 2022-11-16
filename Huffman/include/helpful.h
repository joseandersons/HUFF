#ifndef HELPFUL_H
#define HELPFUL_H

#include <stdio.h>
#include <stdlib.h>

#define ERROR "\033[1;38;5;1m"
#define WHITE_INTENSE "\033[1;38;5;15m"
#define WHITE "\033[38;5;15m"
#define GREEN "\033[1;38;5;28m"
#define YELLOW "\033[1;38;5;220m"
#define BLUE "\033[38;5;220m"
#define RESET "\033[0m"
#define REVEAL "\033[28m"
#define ORANGE "\033[38;5;214m"
#define BLINK "\033[6m"
#define S_BLINK "\033[25m"
#define ITALIC "\033[3;2m"

int is_bit_set(unsigned char c, int i);
void error(char *msg);
void binary_print(int teste);
unsigned char set_bit(unsigned char c, int i);

#endif