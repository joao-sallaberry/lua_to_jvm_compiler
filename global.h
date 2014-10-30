#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdlib.h>

extern const char * const operators[];
extern size_t size_operators;
int operator_pos(char *);

extern const char * const keywords[];
extern size_t size_keywords;
int keyword_pos(char * );
void print_keyword_table();

int search_insert_sym(char* value);
void print_symbol_table();

int search_insert_float(float value);
void print_float_table();

#endif
