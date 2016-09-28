all: mythread.c
	@gcc -c mythread.c
	@ar rcs mythread.a mythread.o
	
clean:
	@rm -f *.o 
