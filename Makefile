CC = gcc
CFLAGS = -O2 -Wall -std=c99

main: token.o lexer.o

clean:
	-rm -f *.o
	-rm -f main
