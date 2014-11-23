#include <stdio.h>
#include <string.h>
#include "token.h"
#include "global.h"

#include "lexer.h"


// define states
typedef enum {ST_INITIAL, ST_COMMENT, ST_NUMBER, ST_FLOAT, ST_ALPHANUM, ST_SPECIALC, ST_COMPARE, NUM_STATES} state_t;
typedef enum {EN_DIGIT, EN_ALPHA, EN_DOT, EN_SPECIALC, EN_COMPARE, EN_EQUAL, EN_HASHTAG, EN_NEWLINE, EN_SPACE, NUM_ENTRIES} entry_type_t;

state_t const next_state[][NUM_ENTRIES] = {
// receive        digit         alpha         dot           specialc     compare     '='         hashtag      newline      space 
/* INITIAL */    {ST_NUMBER,   ST_ALPHANUM,   ST_FLOAT,     ST_SPECIALC, ST_COMPARE, ST_COMPARE, ST_COMMENT,  ST_INITIAL,  ST_INITIAL},
/* COMMENT */    {ST_COMMENT,  ST_COMMENT,    ST_COMMENT,   ST_COMMENT,  ST_COMMENT, ST_COMMENT, ST_COMMENT,  ST_INITIAL,  ST_COMMENT},
/* NUMBER */     {ST_NUMBER,   -1,            ST_FLOAT,     -1,          -1,         -1,         -1,          -1,          -1},
/* FLOAT */      {ST_FLOAT,    -1,            -1,           -1,          -1,         -1,         -1,          -1,          -1},
/* ALPHANUM */   {ST_ALPHANUM, ST_ALPHANUM,   -1,           -1,          -1,         -1,         -1,          -1,          -1},
/* SPECIALC */   {-1,          -1,            -1,           -1,          -1,         -1,         -1,          -1,          -1},
/* COMPARE */    {-1,          -1,            -1,           -1,          -1,         ST_COMPARE, -1,          -1,          -1} 
};

// declare state actions
void do_nothing();
void do_initial_digit();
void do_initial_alpha();
void do_initial_dot();
void do_initial_specialc();
void do_number_digit();
void do_number_dot();
void do_float_digit();
void do_alphanum_digit();
void do_alphanum_alpha();
void do_compare_equal();

void (*const action_table [NUM_STATES][NUM_ENTRIES]) (void) = {
//                digit              alpha              dot             specialc             compare              '='                  hashtag      newline      space
/* INITIAL */    {do_initial_digit,  do_initial_alpha,  do_initial_dot, do_initial_specialc, do_initial_specialc, do_initial_specialc, do_nothing,  do_nothing,  do_nothing},
/* COMMENT */    {do_nothing,        do_nothing,        do_nothing,     do_nothing,          do_nothing,          do_nothing,          do_nothing,  do_nothing,  do_nothing},
/* NUMBER */     {do_number_digit,   do_nothing,        do_number_dot,  do_nothing,          do_nothing,          do_nothing,          do_nothing,  do_nothing,  do_nothing},
/* FLOAT */      {do_float_digit,    do_nothing,        do_nothing,     do_nothing,          do_nothing,          do_nothing,          do_nothing,  do_nothing,  do_nothing},
/* ALPHANUM */   {do_alphanum_digit, do_alphanum_alpha, do_nothing,     do_nothing,          do_nothing,          do_nothing,          do_nothing,  do_nothing,  do_nothing},
/* SPECIALC */   {do_nothing,        do_nothing,        do_nothing,     do_nothing,          do_nothing,          do_nothing,          do_nothing,  do_nothing,  do_nothing},
/* COMPARE */    {do_nothing,        do_nothing,        do_nothing,     do_nothing,          do_nothing,          do_compare_equal,    do_nothing,  do_nothing,  do_nothing}
};


// auxiliar functions
int is_digit(char c) {
    return c >= '0' && c <= '9';
}
int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
char * is_specialc(char c) {
    return strchr(",;'+-*/()[]&|", c);
}
char * is_compare(char c) {
    return strchr("<>!", c);
}

unsigned int line = 1, column = 0;
unsigned int token_line, token_column = 0;

entry_type_t classify_entry(char c) {
    if (c == ' ' || c == '\t')
	return EN_SPACE;
    else if (is_digit(c))
	return EN_DIGIT;
    else if (is_alpha(c))
	return EN_ALPHA;
    else if (is_specialc(c))
	return EN_SPECIALC;
    else if (is_compare(c))
	return EN_COMPARE;
    else if (c == '=')
	return EN_EQUAL;
    else if (c == '.')
	return EN_DOT;
    else if (c == '#')
	return EN_HASHTAG;
    else if (c == '\n') {
	line++;
	column = 0;
	return EN_NEWLINE;
    }
    else
	fprintf(stderr, "entry char is of unknown type\n");
    return 1;
}


char buffer[256];
size_t buffer_pt;
int buffer_int;
float buffer_fl;
char buffer_sym;
token_type_t token_type;

state_t current_state = ST_INITIAL;

token_t *make_token() {
    token_t *t = NULL;
    switch (current_state) {
    case ST_NUMBER:
	t = add_int_token(buffer_int);	
	break;
    case ST_FLOAT:
	t = add_float_token(buffer_int + buffer_fl);
	break;
    case ST_ALPHANUM: // TODO create state for receiving digits
	buffer[buffer_pt] = 0;
	t = add_alphanum_token(buffer);
	break;
    case ST_SPECIALC:
    case ST_COMPARE:
	t = add_operator_token(buffer);
	break;
    default:
	fprintf(stderr, "ERROR: token being created in wrong state\n");
    }
    t->column = token_column;
    t->line = token_line; 
    return t;
}

char current_char = '\t';
char next_char = '\t';

token_t *get_next_token(FILE *f) {
    current_state = ST_INITIAL;

    while (1) {
	current_char = getc(f);//current_char = next_char;
	column++;
	if (current_char == EOF)
	    return NULL;
        //next_char = getc(f);
	entry_type_t cur_char_type = classify_entry(current_char);
	
	action_table[current_state][cur_char_type]();
	if (next_state[current_state][cur_char_type] == -1) {
	    ungetc(current_char, f);
	    column--;
	    return make_token();
	}
	current_state = next_state[current_state][cur_char_type];
        //printf("-%d-", current_state);
    }
}

void save_position() {
    token_line = line;
    token_column = column;
}


//** define state actions **//

void do_nothing() {
}

void do_initial_digit() {
    buffer_int = 0;
    do_number_digit();
    save_position();
}

void do_initial_alpha() {
    buffer_pt = 0;
    do_alphanum_alpha();
    save_position();
}

unsigned int fl_divider;
void do_initial_dot() {
    buffer_int = 0;
    buffer_fl = 0;
    fl_divider = 1;
    save_position();
}

void do_initial_specialc() {
    buffer[0] = current_char;
    buffer[1] = 0;
    buffer_pt = 1;
    save_position();
}

void do_number_digit() {
    buffer_int *= 10;
    buffer_int += current_char - '0';
}

void do_number_dot() {
    buffer_fl = 0;
    fl_divider = 1;
}

void do_float_digit() {
    fl_divider *= 10;
    buffer_fl += (float)(current_char - '0') / fl_divider;
}

void do_alphanum_digit() {
    do_alphanum_alpha();
}

void do_alphanum_alpha() {
    buffer[buffer_pt++] = current_char;
}

void do_compare_equal() {
    buffer[buffer_pt++] = current_char;
    buffer[buffer_pt] = 0;
}
