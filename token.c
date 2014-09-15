#include "token.h"

#include <stdio.h>
#include <stdlib.h>

void init_token_list() {
    token_list = NULL;
    last_token = NULL;
}

token_t * get_token_list() {
    return token_list;
}

void add_token(token_type_t type, char* value) {
    token_t * t = malloc(sizeof(token_t));
    t->type = type;
    t->value = value;
    t->next = NULL;

    if (!token_list)
        token_list = t;
    else
        last_token->next = t; //TODO check debugger
    last_token = t;
}

void print_token_list() {
    token_t * t = token_list;

    while (t) {
        printf("type=%d value=%s\n", t->type, t->value);
        t = t->next;   
    }
}



