#include "lexer.h"
#include "token.h"
#include "global.h"

int main() {
    run_lexer("ENTRADA.txt");

    print_token_list();
    print_symbol_table();
    
    return 0;
}
