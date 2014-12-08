#include <stdio.h>
#include "token.h"
#include "syntatic.h"
#include "global.h"

int main() {
    FILE *f;
    f = fopen("../ENTRADA.lua", "r"); // TODO treat return

    int status = analyse(f);
    if (!status)
	printf("ACCEPTED!\n\n");
    else
	printf("NOT ACCEPTED\n\n");


    print_float_table();
    print_keyword_table();
    print_symbol_table();

    
    return 0;
}
