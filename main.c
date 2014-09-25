#include <stdio.h>
#include "lexer.h"
#include "token.h"
#include "global.h"

int main() {
    FILE *f;
    f = fopen("ENTRADA.txt", "r"); // TODO treat return

    token_t *t;

    printf("--- TOKENS ---\n");
    while (t = get_next_token(f)) {
	printf("class=%d l=%2d c=%2d value=%d\n", t->type, t->line, t->column, t->int_value);
    }
    printf("\n");

    print_float_table();
    print_keyword_table();
    print_symbol_table();

    
    return 0;
}
