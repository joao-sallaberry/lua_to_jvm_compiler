#include "token.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"

token_t *token_list = NULL;
token_t *last_token = NULL;

token_t *get_token_list() {
    return token_list;
}

token_t *create_token(token_type_t type) {
    token_t *t = malloc(sizeof(token_t));
    t->type = type;
    t->value = 0;
    t->next = NULL;
    
    if (!token_list)
        token_list = t;
    else
        last_token->next = t; //TODO check debugger
    last_token = t;

    return t;
}

token_t *add_int_token(int i) {
    token_t *t = create_token(TYPE_NUMBER);
    t->value = i;
    return t;
}

token_t *add_float_token(float f) {
    token_t *t = create_token(TYPE_FLOAT);
    t->value = search_insert_float(f);
    return t;
}

token_t *add_alphanum_token(char* value) {
    token_t *t = NULL;
    for (int i = 0; i < size_keywords; i++)
	if (!strcmp(value, keywords[i])) {
	    t = create_token(TYPE_KEYWORD);
	    t->value = i;
	    return t;
	}
    if (!t) {
	t = create_token(TYPE_IDENTIFIER);
	t->value = search_insert_sym(value);
    }
    return t;
}

token_t *add_operator_token(char* sym) {
    token_t *t = create_token(TYPE_SYMBOL);
    for (int i = 0; ; i++)
	if (!strcmp(sym, operators[i])) {
	    t->value = i;
	    return t;
	}
}
