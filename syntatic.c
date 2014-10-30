#include "syntatic.h"

#include "token.h"
#include "lexer.h"
#include "global.h"
#include <stdlib.h>


typedef enum{FSM_DECLARACAO_VARIAVEL,
	     FSM_TIPO,
	     NUM_FSM
} sub_machine_t; 

int fsm_declaracao_variavel(token_t * t);
int fsm_tipo(token_t * t);

int (*const sub_machines[NUM_FSM]) (token_t * ) = {
    fsm_declaracao_variavel,
    fsm_tipo
};


typedef struct stack {
    sub_machine_t sub_machine;
    int state;
    struct stack * next;
} stack_t;


sub_machine_t sub_machine;
int state;
stack_t * stack_top = NULL;

// analyse syntax of entry
int analyse(FILE * f) {
    token_t *t;

    sub_machine_t main_machine = sub_machine = FSM_DECLARACAO_VARIAVEL;
    int final_state = 3;
    state = 0;

    printf("--- TOKENS ---\n");
    while (t = get_next_token(f)) {
	printf("class=%d l=%2d c=%2d value=%d\n", t->type, t->line, t->column, t->int_value);
	state = sub_machines[sub_machine](t);
    }
    printf("\n");
    
    if (sub_machine == main_machine && state == final_state)
	return 0;
    else
	return -1;
}

// push current machine and return state to the stack
void push(int ret_st) {
    stack_t * s = malloc(sizeof (stack_t));
    s->sub_machine = sub_machine;
    s->state = ret_st;
    s->next = stack_top;

    stack_top = s;
}

// pop return machine and state from the stack
int pop() {
    if (stack_top == NULL)
	return -1; //TODO: error
    
    int st = stack_top->state;
    sub_machine = stack_top->sub_machine;
    free(stack_top);
    stack_top = stack_top->next;
    return st;
}

// call a sub-machine and execute semantic actions
int call_sm(sub_machine_t sm, int ret_st) {
    push(ret_st);
    sub_machine = sm;
    return 0; // state on new sm
}

// semantic action
void semantico_tbd() {
    printf("TODO -- sm:%2d  state:%2d\n", sub_machine, state);
}

//*** sub-machines ***//
int fsm_declaracao_variavel(token_t * t) { //TODO: unfinished
    switch (state) {
    case 0:
	semantico_tbd();
	return call_sm(FSM_TIPO, 1);

    case 1:
	if (t->type == TYPE_IDENTIFIER) {
	    semantico_tbd();
	    return 2;
	}
	break;

    case 2:
	if (t->type == TYPE_SYMBOL && t->int_value == operator_pos(";")) {
	    semantico_tbd();
	    return 3;
	}
	break;

    case 3:
	return pop();
	
    }
    return -1;
}

int fsm_tipo(token_t * t) {
    switch (state) {
    case 0:
	if (t->type == TYPE_KEYWORD && (
		    t->int_value == keyword_pos("int") ||
		    t->int_value == keyword_pos("float") ||
	            t->int_value == keyword_pos("bool") ||
	            t->int_value == keyword_pos("char"))) {
	    semantico_tbd();
	    return 1;
	}
	break;
	
    case 1:
	return pop();
	
    }
    return -1;
}
