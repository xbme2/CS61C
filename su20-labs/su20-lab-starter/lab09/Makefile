CC=gcc
CFLAGS=--std=c99 -Wall

simd: simd.o common.o
	${CC} ${CFLAGS} -o simd simd.o common.o

simd.o:	common.h

common.o: common.h


.PHONY : clean
clean:
	-rm -f simd
	-rm -f *.o
