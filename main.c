#include <stdio.h>
#include <string.h>
#include "token.h"

// define states
typedef enum {ST_INITIAL, ST_NUMBER, ST_FLOAT, ST_ALPHANUM, ST_SYMBOL, NUM_STATES} state_t;
typedef enum {EN_DIGIT, EN_ALPHA, EN_SYMBOL, EN_DOT, EN_SPACE, NUM_ENTRIES} entry_type_t;

state_t const next_state[][NUM_ENTRIES] = {
// receive        digit         alpha         symbol        dot           space 
/* INITIAL */    {ST_NUMBER,   ST_ALPHANUM,   ST_SYMBOL,    0,            ST_INITIAL},
/* NUMBER */     {ST_NUMBER,   0,             0,            ST_FLOAT,     ST_INITIAL},
/* ALPHANUM */   {ST_ALPHANUM, ST_ALPHANUM,   0,            0,            ST_INITIAL},
/* SYMBOL */     {0,           0,             ST_SYMBOL,    0,            ST_INITIAL}
};


// declare state actions
void do_nothing();
void do_initial_digit();
void do_initial_alpha();
void do_initial_symbol();
void do_number_digit();
void do_number_dot();
void do_number_space();
void do_float_digit();
void do_float_space();
void do_alphanum_digit();
void do_alphanum_alpha();
void do_alphanum_space();
void do_symbol_symbol();
void do_symbol_space();

void (*const action_table [NUM_STATES][NUM_ENTRIES]) (void) = {
//                digit                alpha                dot             symbol                 space
/* INITIAL */    {do_initial_digit,    do_initial_alpha,    do_nothing,     do_initial_symbol,     do_nothing},
/* NUMBER */     {do_number_digit,     do_nothing,          do_nothing,     do_nothing,            do_number_space},
/* FLOAT */      {do_float_digit,      do_nothing,          do_nothing,     do_nothing,            do_float_space},
/* ALPHANUM */   {do_alphanum_digit,   do_alphanum_alpha,   do_nothing,     do_nothing,            do_alphanum_space},
/* SYMBOL */     {do_nothing,          do_nothing,          do_nothing,     do_symbol_symbol,      do_symbol_space}
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
    else if (c == '.')
	return EN_DOT;
    else
	fprintf(stderr, "entry char is of unknown type\n");
    return 1;
}


char buffer[256];
size_t buffer_pt = 0;
int buffer_int;
float buffer_fl;
char buffer_sym;
token_type_t token_type;

char current_char = ' ';
char next_char = ' ';

int main() {
    state_t current_state = ST_INITIAL;

    FILE *f;
    f = fopen("example.txt", "r"); // TODO treat return

    init_token_list();

    while (1) {
	current_char = next_char;
	if(current_char == EOF)
	    break;
        next_char = getc(f);
	entry_type_t cur_char_type = classify_entry(current_char);

	action_table[current_state][cur_char_type]();
	current_state = next_state[current_state][cur_char_type];
        //printf("-%d-", current_state);
    }

    print_token_list();

    return 0;
}

//** define state actions **//

void do_nothing() {
}

void do_initial_digit() {
    buffer_int = 0;
    do_number_digit();
}

void do_initial_alpha() {
    buffer_pt = 0;
    do_alphanum_alpha();
}

void do_initial_symbol() {
    buffer_sym = current_char;
}

void do_number_digit() {
    buffer_int *= 10;
    buffer_int += current_char - '0';
}

void do_number_space() {
    add_int_token(buffer_int);
}

void do_float_digit() {
    buffer_fl +=
}

void do_float_space() {
    add_float_token(buffer_int + buffer_float);
}

void do_alphanum_digit() {
    do_alphanum_alpha();
}

void do_alphanum_alpha() {
    buffer[buffer_pt++] = current_char;
}

void do_alphanum_space() {
    buffer[buffer_pt] = 0;
    add_identifier_token(buffer);
}

void do_symbol_symbol() { //TODO
}

void do_symbol_space() {
    add_symbol_token(buffer_sym);
}

//TODO ask
// como salvar value?
