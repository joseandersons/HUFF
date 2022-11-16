#include "../include/helpful.h"

// Imprime a mensagem de erro caso aconteça:
void error(char *msg){
    printf(ERROR "%s\n", msg);
}

unsigned char set_bit(unsigned char c, int i){
    unsigned char mask = 1 << i;

    return mask | c;
}

int is_bit_set(unsigned char c, int i){
    unsigned char mask = 1 << i;

    return mask & c;
}

void binary_print(int teste){
    int num = teste;
    unsigned char *bin = (unsigned char*)malloc(8);
    int aux;

    for (aux = 7; aux >= 0; aux--) {
        if (num % 2 == 0) {
            bin[aux] = '0';
            num = num / 2;
        }
        else {
            bin[aux] = '1';
            num = num / 2;
        }
    }

    printf("%s", bin);

    free(bin);
}