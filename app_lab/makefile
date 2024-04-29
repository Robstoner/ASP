all: tf

tf: tf.o pgm_io.o
	mpicc tf.o pgm_io.o -o tf

tf.o: tf.c
	mpicc -c tf.c -o tf.o

pgm_io.o: pgm_io.c
	mpicc -c pgm_io.c -o pgm_io.o

clean: 
	rm -f tf tf.o pgm_io.o
