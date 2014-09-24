#include "token.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"

token_t *token_list;
token_t *last_token;

void init_token_list() {
    token_list = NULL;
    last_token = NULL;
}

token_t *get_token_list() {
    return token_list;
}

token_t *create_token(token_type_t type) {
    token_t *t = malloc(sizeof(token_t));
    t->type = type;
    t->int_value = 0;
    t->flo_value = 0;
    t->next = NULL;
    
    if (!token_list)
        token_list = t;
    else
        last_token->next = t; //TODO check debugger
    last_token = t;

    return t;
}

void add_int_token(int i) {
    token_t *t = create_token(TYPE_NUMBER);
    t->int_value = i;
}

void add_float_token(float f) {
    token_t *t = create_token(TYPE_FLOAT);
    t->flo_value = f;
}

void add_keyword_token(char* value) {
    token_t *t = create_token(TYPE_KEYWORD);
    for (int i = 0; i < size_keywords; i++)
	if (!strcmp(value, keywords[i])) {
	    t->int_value = i;
	    return;
	}
}

void add_identifier_token(char* value) {
    token_t *t = create_token(TYPE_IDENTIFIER);
    t->int_value = search_insert_sym(value);
}

void add_specialc_token(char* sym) {
    token_t *t = create_token(TYPE_SYMBOL);
    for (int i = 0; ; i++)
	if (!strcmp(sym, specialc_table[i])) {
	    t->int_value = i;
	    return;
	}
}

void print_token_list() {
    token_t * t = token_list;

    printf("--- TOKEN LIST ---\n");
    while (t) {
	printf("type=%d value=", t->type);
	if (t->type == TYPE_NUMBER)
	    printf("%d", t->int_value);
	else if (t->type == TYPE_FLOAT)
	    printf("%f", t->flo_value);
	else if (t->type == TYPE_KEYWORD)
	    printf("%d", t->int_value);
	else if (t->type == TYPE_IDENTIFIER)
	    printf("%d", t->int_value);
	else if (t->type == TYPE_SYMBOL)
	    printf("%d", t->int_value);
	printf("\n");

        t = t->next;   
    }
    printf("\n");
}



