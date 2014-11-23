#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "global.h"

#include "token.h"

/*** operators ***/
const char * const operators[] = {
    "=", ";", ",", "'",
    "+", "-", "*", "/",
    "(", ")",
    "==", "!=" "<", ">", "<=", ">=",
    "&", "|"
};
size_t size_operators = (sizeof(operators) / sizeof(char*));

// return operator index
int operator_pos(char * str) {
    for (int i = 0; i < size_operators; i++) {
	if (strcmp(operators[i], str) == 0)
	    return i;
    }
    return -1; // not found
}


/*** keywords ***/
const char * const keywords[] = {
    "if", "else", "while", "then", "do", "end", "ret",
    "int", "float", "bool", "char",
    "mod",
    "true", "false",
    "print"
};
size_t size_keywords = (sizeof(keywords) / sizeof(char*));

// return keyword index
int keyword_pos(char * str) {
    for (int i = 0; i < size_keywords; i++) {
	if (strcmp(keywords[i], str) == 0)
	    return i;
    }
    return -1; // not found
}

void print_keyword_table() {
    printf("--- KEYWORD TABLE (class %d) ---\n", TYPE_KEYWORD);
    for (int i = 0; i < size_keywords; i++)
	printf("%4d %s\n", i, keywords[i]);
    printf("\n");
}


/*** symbols ***/
typedef struct symbol {
    char* value;
    struct symbol *next;
} symbol_t;


symbol_t *first_symbol = NULL;
symbol_t *last_symbol = NULL;

int search_insert_sym(char* value) {
    symbol_t *s = first_symbol;
    int pos = 0;

    // search symbol
    while (s) {
	if (!strcmp(value, s->value))
	    return pos;
	s = s->next;
	pos++;
    }

    // insert symbol
    if (!first_symbol) { // empty table
	first_symbol = malloc(sizeof(symbol_t));
	last_symbol = first_symbol;
    }
    else {
	last_symbol->next = malloc(sizeof(symbol_t));
	last_symbol = last_symbol->next;
    }
    last_symbol->value = malloc(strlen(value) + 1);
    strcpy(last_symbol->value, value);
    last_symbol->next = NULL;
    
    return pos;
}

void print_symbol_table() {
    symbol_t *s = first_symbol;
    int pos = 0;
    printf("--- SYMBOL TABLE (class %d) ---\n", TYPE_IDENTIFIER);
    while(s) {
	printf("%4d  %s\n", pos, s->value);
	pos++;
	s = s->next;
    }
    printf("\n");
}


/*** floats ***/
typedef struct float_t {
    float value;
    struct float_t *next;
} float_t;


float_t *first_float = NULL;
float_t *last_float = NULL;

int search_insert_float(float value) {
    float_t *f = first_float;
    int pos = 0;

    // search float
    while (f) {
	if (value == f->value)
	    return pos;
	f = f->next;
	pos++;
    }

    // insert float
    if (!first_float) { // empty table
	first_float = malloc(sizeof(float_t));
	last_float = first_float;
    }
    else {
	last_float->next = malloc(sizeof(float_t));
	last_float = last_float->next;
    }
    last_float->value = value;
    last_float->next = NULL;
    
    return pos;
}

void print_float_table() {
    float_t *f = first_float;
    int pos = 0;
    printf("--- FLOAT TABLE (class %d) ---\n", TYPE_FLOAT);
    while(f) {
	printf("%4d  %f\n", pos, f->value);
	pos++;
	f = f->next;
    }
    printf("\n");
}
