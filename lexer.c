#include <stdio.h>
#include <string.h>
#include "token.h"

#include "lexer.h"


// define states
typedef enum {ST_INITIAL, ST_COMMENT, ST_NUMBER, ST_FLOAT, ST_ALPHANUM, ST_SYMBOL, NUM_STATES} state_t;
typedef enum {EN_DIGIT, EN_ALPHA, EN_DOT, EN_SYMBOL, EN_HASHTAG, EN_NEWLINE, EN_SPACE, NUM_ENTRIES} entry_type_t;

state_t const next_state[][NUM_ENTRIES] = {
// receive        digit         alpha         dot           symbol       hashtag      newline      space 
/* INITIAL */    {ST_NUMBER,   ST_ALPHANUM,   ST_FLOAT,     ST_SYMBOL,   ST_COMMENT,  ST_INITIAL,  ST_INITIAL},
/* COMMENT */    {ST_COMMENT,  ST_COMMENT,    ST_COMMENT,   ST_COMMENT,  ST_COMMENT,  ST_INITIAL,  ST_COMMENT},
/* NUMBER */     {ST_NUMBER,   0,             ST_FLOAT,     0,           0,           ST_INITIAL,  ST_INITIAL},
/* FLOAT */      {ST_FLOAT,    0,             0,            0,           0,           ST_INITIAL,  ST_INITIAL},
/* ALPHANUM */   {ST_ALPHANUM, ST_ALPHANUM,   0,            0,           0,           ST_INITIAL,  ST_INITIAL},
/* SYMBOL */     {0,           0,             0,            ST_SYMBOL,   0,           ST_INITIAL,  ST_INITIAL}
};

// declare state actions
void do_nothing();
void do_initial_digit();
void do_initial_alpha();
void do_initial_dot();
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
//                digit               alpha               dot              symbol              hashtag              newline            space
/* INITIAL */    {do_initial_digit,   do_initial_alpha,   do_initial_dot,  do_initial_symbol,  do_nothing,          do_nothing,        do_nothing},
/* COMMENT */    {do_nothing,         do_nothing,         do_nothing,      do_nothing,         do_nothing,          do_nothing,        do_nothing},
/* NUMBER */     {do_number_digit,    do_nothing,         do_number_dot,   do_nothing,         do_nothing,          do_number_space,   do_number_space},
/* FLOAT */      {do_float_digit,     do_nothing,         do_nothing,      do_nothing,         do_nothing,          do_float_space,    do_float_space},
/* ALPHANUM */   {do_alphanum_digit,  do_alphanum_alpha,  do_nothing,      do_nothing,         do_nothing,          do_alphanum_space, do_alphanum_space},
/* SYMBOL */     {do_nothing,         do_nothing,         do_nothing,      do_symbol_symbol,   do_nothing,          do_symbol_space,   do_symbol_space}
};


// keywords
const char * const keywords[] = {
    "if", "else", "while", "then",
    "int", "float", "bool"
};
size_t size_keywords = (sizeof(keywords) / sizeof(char *));

// auxiliar functions
inline int is_digit(char c) {
    return c >= '0' && c <= '9';
}
inline int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
inline char * is_symbol(char c) {
    return strchr("<>=()", c);
}

entry_type_t classify_entry(char c) {
    if (c == ' ' || c == '\t')
	return EN_SPACE;
    else if (is_digit(c))
	return EN_DIGIT;
    else if (is_alpha(c))
	return EN_ALPHA;
    else if (is_symbol(c))
	return EN_SYMBOL;
    else if (c == '.')
	return EN_DOT;
    else if (c == '#')
	return EN_HASHTAG;
    else if (c == '\n')
	return EN_NEWLINE;
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

char current_char = ' ';
char next_char = ' ';

void run_lexer(char* file_name) {
    state_t current_state = ST_INITIAL;

    FILE *f;
    f = fopen(file_name, "r"); // TODO treat return

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

unsigned int fl_divider;
void do_initial_dot() {
    buffer_int = 0;
    buffer_fl = 0;
    fl_divider = 1;
}

void do_initial_symbol() {
    buffer_sym = current_char;
}

void do_number_digit() {
    buffer_int *= 10;
    buffer_int += current_char - '0';
}

void do_number_dot() {
    buffer_fl = 0;
    fl_divider = 1;
}

void do_number_space() {
    add_int_token(buffer_int);
}

void do_float_digit() {
    fl_divider *= 10;
    buffer_fl += (float)(current_char - '0') / fl_divider;
}

void do_float_space() {
    add_float_token(buffer_int + buffer_fl);
}

void do_alphanum_digit() {
    do_alphanum_alpha();
}

void do_alphanum_alpha() {
    buffer[buffer_pt++] = current_char;
}

void do_alphanum_space() {// TODO create state for receiving digits
    buffer[buffer_pt] = 0;
    for (int i = 0; i < size_keywords; i++) {
	if (!strcmp(buffer, keywords[i])) {
	    add_keyword_token(buffer);
	    return;
	}
    }
    add_identifier_token(buffer);
}

void do_symbol_symbol() { //TODO
}

void do_symbol_space() {
    add_symbol_token(buffer_sym);
}

//TODO ask
// como salvar value?
