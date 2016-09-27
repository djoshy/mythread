all: run.c libtest.c libtest.h
	@rm -f *.a
	@gcc -c libtest.c
	@ar rcs libtest.a libtest.o
	@gcc -o run run.c libtest.a

clean:
	@rm -f run
	@rm -f *.o 

test: test.c libtest.c libtest.h
	@gcc -c libtest.c
	@ar rcs libtest.a libtest.o
	@gcc -o test test.c libtest.a