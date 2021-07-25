CC = cc

.PHONY = all clean

all : seq-k-means

clean :
	rm -rf seq-k-means

seq-k-means : sequential-k-means-algorithm.c
	${CC} sequential-k-means-algorithm.c -o seq-k-means
