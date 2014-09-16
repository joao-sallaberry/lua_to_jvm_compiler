#include <stdio.h>
#include "token.h"


// define states
typedef enum {STATE_INITIAL, STATE_FOO, STATE_SYMBOL, STATE_BAR, STATE_FINAL, NUM_STATES} state_t;
typedef char entry_char_t;
typedef state_t state_action_t();

// declare state actions
state_t do_state_initial();
state_t do_state_foo();
state_t do_state_symbol();
state_t do_state_bar();
state_t do_state_final();

// lookup table of state actions
state_action_t* const state_table[NUM_STATES] = {
    do_state_initial, do_state_foo, do_state_symbol, do_state_bar, do_state_final
};

// execute action and find next state
state_t run_state(state_t current_state) {
    return state_table[current_state]();
};


char buffer[256];
size_t buffer_pt = 0;
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

// initial state
state_t do_state_initial() { //TODO why pointer?
    //printf("!%c!", entry_char);
    if (current_char >= '0' && current_char <= '9') {
        buffer[buffer_pt++] = current_char;
        return STATE_FOO;
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
state_t do_state_foo() {
    if (current_char >= '0' && current_char <= '9') {
        buffer[buffer_pt++] = current_char;
        return STATE_FOO;
    }
    else if (current_char == ' ' || current_char == '\n') {
        buffer[buffer_pt++] = 0;
        add_token(TYPE_NUMBER, buffer);
        buffer_pt = 0;
        return STATE_INITIAL;
    }
    else if (current_char == EOF) {
        return STATE_FINAL;
    }
    return STATE_FOO;
}

// create symbol tokens
state_t do_state_symbol() {
    if (current_char == ' ' || current_char == '\n') {
        buffer[buffer_pt++] = 0;
        add_token(TYPE_SYMBOL, buffer);
        buffer_pt = 0;
        return STATE_INITIAL;
    }
    return STATE_SYMBOL;
}

state_t do_state_bar() {
    return STATE_BAR;
}

state_t do_state_final() {
    return STATE_FINAL;
}


//TODO ask
// como salvar value?
