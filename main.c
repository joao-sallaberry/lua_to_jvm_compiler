#include <stdio.h>
#include "token.h"


// define states
typedef enum {STATE_INITIAL, STATE_FOO, STATE_BAR, NUM_STATES} state_t;
typedef char entry_char_t;
typedef state_t state_action_t(entry_char_t *entry_char);

// declare state actions
state_t do_state_initial(entry_char_t *entry_char);
state_t do_state_foo(entry_char_t *entry_char);
state_t do_state_bar(entry_char_t *entry_char);

// lookup table of state actions
state_action_t* const state_table[NUM_STATES] = {
    do_state_initial, do_state_foo, do_state_bar
};

// execute action and find next state
state_t run_state(state_t current_state, entry_char_t *entry_char) {
    return state_table[ current_state ]( entry_char );
};


int main() {
    state_t current_state = STATE_INITIAL;
    entry_char_t entry_char;

    init_token_list();
    //add_token(TYPE_NUMBER, "asd");

    while (1) {
        scanf("%c", &entry_char); //TODO treat return value
        current_state = run_state(current_state, &entry_char);

    }

    print_token_list();

    return 0;
}

// define state actions
state_t do_state_initial(entry_char_t *entry_char) {
    if (*entry_char == 'a')
        return STATE_FOO;
    return STATE_INITIAL;
}

state_t do_state_foo(entry_char_t *entry_char) {
    printf("lalaa");
}

state_t do_state_bar(entry_char_t *entry_char) {}
