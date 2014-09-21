#include <stdio.h>
#include "token.h"


// define states
typedef enum {STATE_INITIAL, STATE_NUMBER, STATE_IDENTIFIER, STATE_SYMBOL, STATE_FINAL, NUM_STATES} state_t;
typedef char entry_char_t;
typedef state_t state_action_t();

// declare state actions
state_t do_state_initial();
state_t do_state_number();
state_t do_state_identifier();
state_t do_state_symbol();
state_t do_state_final();

// lookup table of state actions
state_action_t* const state_table[NUM_STATES] = {
    do_state_initial, do_state_number, do_state_identifier, do_state_symbol, do_state_final
};

// execute action and find next state
state_t run_state(state_t current_state) {
    return state_table[current_state]();
};


char buffer[256];
size_t buffer_pt = 0;
int buffer_int = 0;
entry_char_t current_char = 0;
entry_char_t next_char = 0;

int main() {
    state_t current_state = STATE_INITIAL;

    FILE *f;
    f = fopen("example.txt", "r"); // TODO treat return

    init_token_list();

    while (1) {
	current_char = next_char;
        next_char = getc(f);
        current_state = run_state(current_state);
        if (current_state == STATE_FINAL) //TODO why not same line?
            break;
        //printf("-%d-", current_state);
    }

    print_token_list();

    return 0;
}

//** define state actions **//

char symbol_list[] = ";=<>";

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


//TODO ask
// como salvar value?
