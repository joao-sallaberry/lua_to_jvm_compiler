#include <stdio.h>
#include "lexer.h"
#include "token.h"
#include "global.h"

int main() {
    FILE *f;
    f = fopen("ENTRADA.txt", "r"); // TODO treat return

    token_t *t;

    while (t = get_next_token(f)) {
    	printf("%d\n", t->type);
    }    

    print_token_list();
    print_float_table();
    print_keyword_table();
    print_symbol_table();

    
    return 0;
}
