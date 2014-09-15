CC = gcc
CFLAGS = -O2

main: token.o

clean:
	-rm -f *.o
	-rm -f main
