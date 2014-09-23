#include "lexer.h"
#include "token.h"


int main() {
    run_lexer("ENTRADA.txt");

    print_token_list();

    return 0;
}
