all: run.c
	@rm *.a
	@gcc -c libtest.c
	@ar rcs libtest.a libtest.o
	@gcc -o run run.c libtest.a

clean:
	@rm -f run
	@rm -f *.o 

	