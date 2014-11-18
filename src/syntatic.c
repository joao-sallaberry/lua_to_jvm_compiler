#include "syntatic.h"

#include "token.h"
#include "lexer.h"
#include "global.h"
#include <stdlib.h>


#define MAIN_MACHINE    FSM_INSTRUCAO
#define MAIN_FINAL_ST   6

typedef enum{FSM_DECLARACAO_VARIAVEL,
	     FSM_DECLARACAO_VAR_COM_ATRIB,
	     FSM_INSTRUCAO,
	     FSM_LACO,
	     FSM_CONDICIONAL,
	     FSM_EXPRESSAO,
	     FSM_TERMO_E,
	     FSM_TERMO_IGUALDADE,
	     FSM_TERMO_RELACIONAL,
	     FSM_TERMO_ADITIVO,
	     FSM_TERMO_MULTIPLICATIVO,
	     FSM_TERMO_PRIMARIO,
	     FSM_TIPO,
	     FSM_RETORNO,
	     NUM_FSM
} sub_machine_t; 

int fsm_declaracao_variavel(token_t * t);
int fsm_declaracao_var_com_atrib(token_t * t);
int fsm_instrucao(token_t * t);
int fsm_laco(token_t * t);
int fsm_condicional(token_t * t);
int fsm_expressao(token_t * t);
int fsm_termo_e(token_t * t);
int fsm_termo_igualdade(token_t * t);
int fsm_termo_relacional(token_t * t);
int fsm_termo_aditivo(token_t * t);
int fsm_termo_multiplicativo(token_t * t);
int fsm_termo_primario(token_t * t);
int fsm_retorno(token_t * t);
int fsm_tipo(token_t * t);

int (*const sub_machines[NUM_FSM]) (token_t * ) = {
    fsm_declaracao_variavel,
    fsm_declaracao_var_com_atrib,
    fsm_instrucao,
    fsm_laco,
    fsm_condicional,
    fsm_expressao,
    fsm_termo_e,
    fsm_termo_igualdade,
    fsm_termo_relacional,
    fsm_termo_aditivo,
    fsm_termo_multiplicativo,
    fsm_termo_primario,
    fsm_retorno,
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
int get_token_flag = 1;

// get next token if sub-machine didn't change
token_t *get_token_cond(FILE * f) {
    static token_t *t;

    if (get_token_flag) {
	t = get_next_token(f);
	if (t) printf("token class=%d l=%2d c=%2d value=%d\n", t->type, t->line, t->column, t->value);
    }
    get_token_flag = 1;

    return t;
}

// analyse syntax of entry
int analyse(FILE * f) {
    token_t *t;

    sub_machine_t main_machine = sub_machine = MAIN_MACHINE;
    int final_state = MAIN_FINAL_ST;
    state = 0;

    printf("--- syntax check ---\n");
    while ((t = get_token_cond(f))) {
	state = sub_machines[sub_machine](t);
	if (state == -1) break;
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

    get_token_flag = 0;
}

// pop return machine and state from the stack
int pop() {
    if (stack_top == NULL)
	return -1; //TODO: error
    
    int st = stack_top->state;
    sub_machine = stack_top->sub_machine;
    free(stack_top);
    stack_top = stack_top->next;

    get_token_flag = 0;

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


/**********************************

           SUB-MACHINES

***********************************/
int fsm_declaracao_variavel(token_t * t) {
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
	if (t->type == TYPE_SYMBOL && t->value == operator_pos(";")) {
	    semantico_tbd();
	    return 3;
	}
	break;

    case 3:
	return pop();
	
    }
    return -1;
}

int fsm_declaracao_var_com_atrib(token_t * t) { //TODO unfinished
    switch (state) {
    case 0:
	if (t->type == TYPE_IDENTIFIER) {
	    semantico_tbd();
	    return 1;
	}
	break;

    case 2:
	if (t->type == TYPE_SYMBOL && t->value == operator_pos(";")) {
	    semantico_tbd();
	    return 3;
	}
	break;

    case 3:
	return pop();
	
    }
    return -1;
}

int fsm_instrucao(token_t * t) {
    switch (state) {
    case 0:
	if (t->type == TYPE_KEYWORD && (
		    t->value == keyword_pos("int") ||
		    t->value == keyword_pos("float") ||
	            t->value == keyword_pos("bool") ||
	            t->value == keyword_pos("char"))) {
	    semantico_tbd();
	    return 1;
	}
	else if (t->type == TYPE_KEYWORD && t->value == keyword_pos("ret")) {
	    semantico_tbd();
	    return 2;
	}
	else if (t->type == TYPE_KEYWORD && t->value == keyword_pos("while")) {
	    semantico_tbd();
	    return 3;
	}
	else if (t->type == TYPE_KEYWORD && t->value == keyword_pos("if")) {
	    semantico_tbd();
	    return 4;
	}
	else if (t->type == TYPE_IDENTIFIER) {
	    semantico_tbd();
	    return 5;
	}
	break;

    case 1:
	semantico_tbd();
	return call_sm(FSM_DECLARACAO_VAR_COM_ATRIB, 6);

    case 2:
	semantico_tbd();
	return call_sm(FSM_RETORNO, 6);

    case 3:
	semantico_tbd();
	return call_sm(FSM_LACO, 6);

    case 4:
	semantico_tbd();
	return call_sm(FSM_CONDICIONAL, 6);
	
    case 5:
	if (t->type == TYPE_SYMBOL && t->value == operator_pos("=")) {
	    semantico_tbd();
	    return 7;
	}
	else if (t->type == TYPE_SYMBOL && t->value == operator_pos("(")) {
	    semantico_tbd();
	    return 8;
	}
	break;

    case 6:
	semantico_tbd();
	return pop();

    case 7:
	semantico_tbd();
	return call_sm(FSM_EXPRESSAO, 10);

    case 8:
	if (t->type == TYPE_SYMBOL && t->value == operator_pos(")")) {
	    semantico_tbd();
	    return 10;
	}
	semantico_tbd();
	return call_sm(FSM_EXPRESSAO, 9);

    case 9:
	if (t->type == TYPE_SYMBOL && t->value == operator_pos(",")) {
	    semantico_tbd();
	    return 11;
	}
	else if (t->type == TYPE_SYMBOL && t->value == operator_pos(")")) {
	    semantico_tbd();
	    return 10;
	}
	break;

    case 10:
	if (t->type == TYPE_SYMBOL && t->value == operator_pos(";")) {
	    semantico_tbd();
	    return 6;
	}
	break;

    case 11:
	semantico_tbd();
	return call_sm(FSM_EXPRESSAO, 9);

    }
    return -1;
}

int fsm_laco(token_t * t) {
    switch (state) {
    case 0:
	semantico_tbd();
	return call_sm(FSM_EXPRESSAO, 1);

    case 1:
	if (t->type == TYPE_KEYWORD && t->value == keyword_pos("do")) {
	    semantico_tbd();
	    return 2;
	}
	break;

    case 2:
	if (t->type == TYPE_KEYWORD && t->value == keyword_pos("end")) {
	    semantico_tbd();
	    return 3;
	}
	semantico_tbd();
	return call_sm(FSM_INSTRUCAO, 2);

    case 3:
	semantico_tbd();
	return pop();

    }
    return -1;
}

int fsm_condicional(token_t * t) {
    switch (state) {
    case 0:
	semantico_tbd();
	return call_sm(FSM_EXPRESSAO, 1);

    case 1:
	if (t->type == TYPE_KEYWORD && t->value == keyword_pos("then")) {
	    semantico_tbd();
	    return 2;
	}
	break;

    case 2:
	if (t->type == TYPE_KEYWORD && t->value == keyword_pos("end")) {
	    semantico_tbd();
	    return 4;
	}
	else if (t->type == TYPE_KEYWORD && t->value == keyword_pos("else")) {
	    semantico_tbd();
	    return 3;
	}
	semantico_tbd();
	return call_sm(FSM_INSTRUCAO, 2);

    case 3:
	if (t->type == TYPE_KEYWORD && t->value == keyword_pos("end")) {
	    semantico_tbd();
	    return 4;
	}
	semantico_tbd();
	return call_sm(FSM_INSTRUCAO, 2);

    case 4:
	semantico_tbd();
	return pop();

    }
    return -1;
}

int fsm_expressao(token_t * t) {
    switch (state) {
    case 0:
	semantico_tbd();
	return call_sm(FSM_TERMO_E, 1);

    case 1:
	if (t->type == TYPE_SYMBOL && t->value == operator_pos("||")) {
	    semantico_tbd();
	    return 2;
	}
	semantico_tbd();
	return pop();

    case 2:
	semantico_tbd();
	return call_sm(FSM_EXPRESSAO, 3);

    case 3:
	semantico_tbd();
	return pop();

    }
    return -1;
}

int fsm_termo_e(token_t * t) {
    switch (state) {
    case 0:
	semantico_tbd();
	return call_sm(FSM_TERMO_IGUALDADE, 1);

    case 1:
	if (t->type == TYPE_SYMBOL && t->value == operator_pos("&&")) {
	    semantico_tbd();
	    return 2;
	}
	semantico_tbd();
	return pop();

    case 2:
	semantico_tbd();
	return call_sm(FSM_TERMO_E, 3);

    case 3:
	semantico_tbd();
	return pop();

    }
    return -1;
}

int fsm_termo_igualdade(token_t * t) {
    switch (state) {
    case 0:
	semantico_tbd();
	return call_sm(FSM_TERMO_RELACIONAL, 1);

    case 1:
	if (t->type == TYPE_SYMBOL && (t->value == operator_pos("==") || t->value == operator_pos("!="))) {
	    semantico_tbd();
	    return 2;
	}
	semantico_tbd();
	return pop();

    case 2:
	semantico_tbd();
	return call_sm(FSM_TERMO_IGUALDADE, 3);

    case 3:
	semantico_tbd();
	return pop();

    }
    return -1;
}

int fsm_termo_relacional(token_t * t) {
    switch (state) {
    case 0:
	semantico_tbd();
	return call_sm(FSM_TERMO_ADITIVO, 1);

    case 1:
	if (t->type == TYPE_SYMBOL && (t->value == operator_pos(">") || t->value == operator_pos("<") || t->value == operator_pos(">=") || t->value == operator_pos("<="))) {
	    semantico_tbd();
	    return 2;
	}
	semantico_tbd();
	return pop();

    case 2:
	semantico_tbd();
	return call_sm(FSM_TERMO_RELACIONAL, 3);

    case 3:
	semantico_tbd();
	return pop();

    }
    return -1;
}

int fsm_termo_aditivo(token_t * t) {
    switch (state) {
    case 0:
	semantico_tbd();
	return call_sm(FSM_TERMO_MULTIPLICATIVO, 1);

    case 1:
	if (t->type == TYPE_SYMBOL && (t->value == operator_pos("+") || t->value == operator_pos("-"))) {
	    semantico_tbd();
	    return 2;
	}
	semantico_tbd();
	return pop();

    case 2:
	semantico_tbd();
	return call_sm(FSM_TERMO_ADITIVO, 3);

    case 3:
	semantico_tbd();
	return pop();

    }
    return -1;
}

int fsm_termo_multiplicativo(token_t * t) {
    switch (state) {
    case 0:
	semantico_tbd();
	return call_sm(FSM_TERMO_PRIMARIO, 1);

    case 1:
	if ((t->type == TYPE_SYMBOL && (t->value == operator_pos("*") || t->value == operator_pos("/"))) ||
	    (t->type == TYPE_KEYWORD && t->value == keyword_pos("mod")) ) {
	    semantico_tbd();
	    return 2;
	}
	semantico_tbd();
	return pop();

    case 2:
	semantico_tbd();
	return call_sm(FSM_TERMO_MULTIPLICATIVO, 3);

    case 3:
	semantico_tbd();
	return pop();

    }
    return -1;
}

int fsm_termo_primario(token_t * t) {
    switch (state) {
    case 0:
	if (t->type == TYPE_IDENTIFIER) {
	    semantico_tbd();
	    return 1;
	}
	else if (t->type == TYPE_NUMBER ||
		 t->type == TYPE_FLOAT ||
		 (t->type == TYPE_KEYWORD && (t->value == keyword_pos("true") || t->value == keyword_pos("false")))) {
	    semantico_tbd();
	    return 2;
	}
	else if (t->type == TYPE_SYMBOL && t->value == operator_pos("(")) {
	    semantico_tbd();
	    return 3;
	}
	break;

    case 1:
	if (t->type == TYPE_SYMBOL && t->value == operator_pos("(")) {
	    semantico_tbd();
	    return 4;
	}
	semantico_tbd();
	return pop();

    case 2:
	semantico_tbd();
	return pop();

    case 3:
      	semantico_tbd();
	return call_sm(FSM_EXPRESSAO, 5);

    case 4:
        if (t->type == TYPE_SYMBOL && t->value == operator_pos(")")) {
	    semantico_tbd();
	    return 2;
	}
	semantico_tbd();
	return call_sm(FSM_EXPRESSAO, 6);

    case 5:
	if (t->type == TYPE_SYMBOL && t->value == operator_pos(")")) {
	    semantico_tbd();
	    return 2;
	}
    break;

    case 6:
	if (t->type == TYPE_SYMBOL && t->value == operator_pos(")")) {
	    semantico_tbd();
	    return 2;
	}
	else if (t->type == TYPE_SYMBOL && t->value == operator_pos(",")) {
	    semantico_tbd();
	    return 7;
	}
	break;

    case 7:
	semantico_tbd();
	return call_sm(FSM_EXPRESSAO, 6);
	
    }
    return -1;
}

int fsm_retorno(token_t * t) {
    switch (state) {
    case 0:
	semantico_tbd();
	return call_sm(FSM_EXPRESSAO, 1);

    case 1:
	if (t->type == TYPE_SYMBOL && t->value == operator_pos(";")) {
	    semantico_tbd();
	    return 2;
	}
	break;

    case 2:
	semantico_tbd();
	return pop();
    }
       
    return -1;
}

int fsm_tipo(token_t * t) {
    switch (state) {
    case 0:
	if (t->type == TYPE_KEYWORD && (
		    t->value == keyword_pos("int") ||
		    t->value == keyword_pos("float") ||
	            t->value == keyword_pos("bool") ||
	            t->value == keyword_pos("char"))) {
	    semantico_tbd();
	    return 1;
	}
	break;
	
    case 1:
	semantico_tbd();
	return pop();
	
    }
    return -1;
}
