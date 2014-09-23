CC = gcc
CFLAGS = -O2 -Wall

main: token.o lexer.o

clean:
	-rm -f *.o
	-rm -f main
