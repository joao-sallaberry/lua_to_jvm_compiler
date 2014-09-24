#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdlib.h>

extern const char * const specialc_table[];

extern const char * const keywords[];
size_t size_keywords;

int search_insert_sym(char* value);
void print_symbol_table();

#endif
