CC= g++ 
CCFLAGS = --std=c++11


default: \
	matrix \

matrix: matrix-factorization.cpp matrix-factorization.h util.h 
	$(CC) $(CCFLAGS) matrix-factorization.cpp -o matrix

exec: matrix
	time ./matrix ratings_syn100.txt 4 0.4 > log2

clean:
	rm -f matrix\
