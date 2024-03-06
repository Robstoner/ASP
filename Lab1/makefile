
CC = gcc
CFLAGS = -Wall
OBJECTS = vanderpol.o gplot.o
LIBS = -lm

all: vanderpol

vanderpol: $(OBJECTS)
	$(CC) $(CFLAGS) -o vanderpol $(OBJECTS) $(LIBS)

vanderpol.o: vanderpol.c gplot.o
	$(CC) $(CFLAGS) -c vanderpol.c

gplot.o: gplot.c gplot.h
	$(CC) $(CFLAGS) -c gplot.c

clean:
	rm -f vanderpol $(OBJECTS)
