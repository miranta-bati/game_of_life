CC = gcc
OFLAGS = -Wall -o
CFLAGS = -Wall -c
LDFLAGS=-pthread

all: gameoflife

gameoflife.o: gameoflife.c
	$(CC) $(CFLAGS) gameoflife.c 

gameoflife: gameoflife.o
	$(CC) $(OFLAGS) gameoflife gameoflife.o $(LDFLAGS)

clean:
	rm -f *.o

distclean:
	rm -f *.o gameoflife
