
CC = gcc
CFLAGS = -Wall
OBJECTS = vanderpol.o gplot.o
OBJECTS2 = my_function.o gplot.o
LIBS = -lm -lgsl -lgslcblas

# all: my_function

# my_function: $(OBJECTS2)
# 	$(CC) $(CFLAGS) -o my_function $(OBJECTS2) $(LIBS)

# my_function.o: my_function.c gplot.o	
# 	$(CC) $(CFLAGS) -c my_function.c

all: vanderpol

vanderpol: $(OBJECTS)
	$(CC) $(CFLAGS) -o vanderpol $(OBJECTS) $(LIBS)

vanderpol.o: vanderpol.c gplot.o
	$(CC) $(CFLAGS) -c vanderpol.c

gplot.o: gplot.c gplot.h
	$(CC) $(CFLAGS) -c gplot.c

clean:
	rm -f $(OBJECTS) $(OBJECTS2)

cleanall:
	rm -f $(OBJECTS2) $(OBJECTS) vanderpol.dat vanderpol my_function
