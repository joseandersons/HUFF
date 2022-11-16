#ifndef MAIN_PANEL_H
#define MAIN_PANEL_H

#define PROGRAM "CafeZip"
#define PROGRAM_LOWERCASE "cafezip"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


void fatal();
void usage();
short parse_options(char *argv[]);
char *get_new_name(int argc, char *argv[], short options);
_Bool select_options(int argc, char *argv[], short options);

#endif