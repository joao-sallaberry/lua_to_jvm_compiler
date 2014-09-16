#include <stdio.h>
#include "token.h"


// define states
typedef enum {STATE_INITIAL, STATE_FOO, STATE_BAR, STATE_FINAL, NUM_STATES} state_t;
typedef char entry_char_t;
typedef state_t state_action_t(entry_char_t *entry_char);

// declare state actions
state_t do_state_initial(entry_char_t *entry_char);
state_t do_state_foo(entry_char_t *entry_char);
state_t do_state_bar(entry_char_t *entry_char);
state_t do_state_final(entry_char_t *entry_char);

// lookup table of state actions
state_action_t* const state_table[NUM_STATES] = {
    do_state_initial, do_state_foo, do_state_bar, do_state_final
};

// execute action and find next state
state_t run_state(state_t current_state, entry_char_t *entry_char) {
    return state_table[current_state]( entry_char );
};


char buffer[256];
size_t buffer_pt = 0;
entry_char_t entry_char;

int main() {
    state_t current_state = STATE_INITIAL;
    entry_char_t entry_char;

    FILE *f;
    f = fopen("example.txt", "r"); // TODO treat return

    init_token_list();

    while (1) {
        entry_char = getc(f);
        current_state = run_state(current_state, &entry_char);
        if (current_state == STATE_FINAL) //TODO why not same line?
            break;
        //printf("-%d-", current_state);
    }

    print_token_list();

    return 0;
}

// define state actions
state_t do_state_initial(entry_char_t *entry_char) { //TODO why pointer?
    //printf("!%c!", *entry_char);
    if (*entry_char >= '0' && *entry_char <= '9') {
        buffer[buffer_pt++] = *entry_char;
        return STATE_FOO;
    }
    else if (*entry_char == EOF) {
        return STATE_FINAL;
    }
    return STATE_INITIAL;
}

state_t do_state_foo(entry_char_t *entry_char) {
    if (*entry_char >= '0' && *entry_char <= '9') {
        buffer[buffer_pt++] = *entry_char;
        return STATE_FOO;
    }
    else if (*entry_char == ' ' || *entry_char == '\n') {
        buffer[buffer_pt++] = 0;
        add_token(TYPE_NUMBER, buffer);
        buffer_pt = 0;
        return STATE_INITIAL;
    }
    else if (*entry_char == EOF) {
        return STATE_FINAL;
    }
}

state_t do_state_bar(entry_char_t *entry_char) {}

state_t do_state_final(entry_char_t *entry_char) {}
