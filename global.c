#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* special chars */
const char * const specialc_table[] = {"=", "<", ">", ";"};


/* keywords */
const char * const keywords[] = {
    "if", "else", "while", "then",
    "int", "float", "bool"
};
size_t size_keywords = (sizeof(keywords) / sizeof(char *));


/* symbols */
typedef struct symbol {
    char* value;
    struct symbol *next;
} symbol_t;


symbol_t *first_symbol = NULL;
symbol_t *last_symbol = NULL;
//size_t sym_tb_size;

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
    last_symbol->value = value;
    //strcpy(last_symbol->value, value);
    last_symbol->next = NULL;
    
    return pos;
}

void print_symbol_table() {
    symbol_t *s = first_symbol;
    int pos = 0;
    printf("--- SYMBOL TABLE ---\n");
    while(s) {
	printf("%2d  %s\n", pos, s->value);
	pos++;
	s = s->next;
    }
    printf("\n");
}
