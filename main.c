#include <stdio.h>
#include <string.h>
#include "token.h"

// define states
typedef enum {ST_INITIAL, ST_NUMBER, ST_ALPHANUM, ST_SYMBOL, NUM_STATES} state_t;
typedef enum {EN_DIGIT, EN_ALPHA, EN_SYMBOL, EN_SPACE, NUM_ENTRIES} entry_type_t;
typedef char entry_char_t;
//typedef state_t state_action_t();

state_t const next_state[][NUM_ENTRIES] = {
// receive        digit         alpha         symbol        space 
/* INITIAL */    {ST_NUMBER,   ST_ALPHANUM,   ST_SYMBOL,    ST_INITIAL},
/* NUMBER */     {ST_NUMBER,   0,             0,            ST_INITIAL},
/* ALPHANUM */   {ST_ALPHANUM, ST_ALPHANUM,   0,            ST_INITIAL},
/* SYMBOL */     {0,           0,             ST_SYMBOL,    ST_INITIAL}
};


// declare state actions
void do_nothing();
void do_initial_digit();
void do_initial_alpha();
void do_initial_symbol();
void do_number_number();
void do_alphanum_number();
void do_alphanum_alpha();
void do_symbol_symbol();

void (*const action_table [NUM_STATES][NUM_ENTRIES]) (void) = {
//                digit                alpha                symbol                 space
/* INITIAL */    {do_initial_digit,    do_initial_alpha,    do_initial_symbol,     do_nothing},
/* NUMBER */     {do_number_number,    do_nothing,          do_nothing,            do_nothing},
/* ALPHANUM */   {do_alphanum_number,  do_alphanum_alpha,   do_nothing,            do_nothing},
/* SYMBOL */     {do_nothing,          do_nothing,          do_symbol_symbol,      do_nothing}
};


// auxiliar functions
inline int is_digit(char c) {
    return c >= '0' && c <= '9';
}
inline int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
inline int is_symbol(char c) {
    return strchr("<>=", c);
}

entry_type_t classify_entry(char c) {
    if (c == ' ' || c == '\n')
	return EN_SPACE;
    else if (is_digit(c))
	return EN_DIGIT;
    else if (is_alpha(c))
	return EN_ALPHA;
    else if (is_symbol(c))
	return EN_SYMBOL;
    else
	fprintf(stderr, "entry char is of unknown type\n");
    return 1;
}


char buffer[256];
size_t buffer_pt = 0;
int buffer_int = 0;
entry_char_t current_char = 0;
entry_char_t next_char = 0;

int main() {
    state_t current_state = ST_INITIAL;

    FILE *f;
    f = fopen("example.txt", "r"); // TODO treat return

    init_token_list();

    while (1) {
	current_char = next_char;
        next_char = getc(f);
	entry_type_t cur_char_type = classify_entry(current_char);

	action_table[current_state][cur_char_type]();
	current_state = next_state[current_state][cur_char_type];
        //if (current_state == STATE_FINAL) //TODO why not same line?
        //    break;
        //printf("-%d-", current_state);
    }

    print_token_list();

    return 0;
}

//** define state actions **//

/*
// initial state
state_t do_state_initial() {
    if (current_char >= '0' && current_char <= '9') {
        buffer_int = current_char - '0';
	if (next_char == ' ' || next_char == '\n') {
	    add_int_token(buffer_int);
	    return STATE_INITIAL;
	}
        return STATE_NUMBER;
    }
    else if (current_char >= 'a' && current_char <= 'z') {
	buffer[buffer_pt++] = current_char;
    }
    else if (current_char == ';') {
	buffer[buffer_pt++] = current_char;
	return STATE_SYMBOL;
    }
    else if (current_char == EOF) {
        return STATE_FINAL;
    }
    return STATE_INITIAL;
}

// state receiving a number
state_t do_state_number() {
    if (current_char >= '0' && current_char <= '9') {
        buffer_int *= 10;
	buffer_int += current_char - '0';
	if (next_char == ' ' || next_char == '\n')
	    return STATE_INITIAL;
        return STATE_NUMBER;
    }
    else if (current_char == EOF) {
        return STATE_FINAL;
    }
    return STATE_NUMBER;
}

// wait next char
state_t do_state_identifier() {
    
    return STATE_IDENTIFIER;
}

// create symbol tokens
state_t do_state_symbol() {
    if (current_char == ' ' || current_char == '\n') {
        buffer[buffer_pt++] = 0;
        //add_token(TYPE_SYMBOL, buffer);
        buffer_pt = 0;
        return STATE_INITIAL;
    }
    return STATE_SYMBOL;
}

state_t do_state_final() {
    return STATE_FINAL;
}
*/

//TODO ask
// como salvar value?
