CC = gcc
CFLAGS = -g -O1 -Wall -std=c99

main: token.o lexer.o global.o

clean:
	-rm -f *.o
	-rm -f main
