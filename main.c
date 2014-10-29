#include <stdio.h>
#include "token.h"
#include "syntatic.h"
#include "global.h"

int main() {
    FILE *f;
    f = fopen("ENTRADA.txt", "r"); // TODO treat return

    int status = analyse(f);
    if (!status)
	printf("ACCEPTED!\n");
    else
	printf("FAILURE\n");


    print_float_table();
    print_keyword_table();
    print_symbol_table();

    
    return 0;
}
