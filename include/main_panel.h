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

#endif