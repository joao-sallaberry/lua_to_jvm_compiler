CC = gcc
CFLAGS = -O2 -Wall

main: token.o

clean:
	-rm -f *.o
	-rm -f main
