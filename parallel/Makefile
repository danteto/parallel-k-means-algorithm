CC = cc

CFLAGS = -O0 -lm -pthread

.PHONY = all clean

all : parallel-k-means

clean :
	rm -rf parallel-k-means

parallel-k-means : parallel-k-means-algorithm.c
	${CC} $(CFLAGS) parallel-k-means-algorithm.c -o parallel-k-means
