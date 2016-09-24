#include "libtest.h"
#include<stdio.h>
#include<stdlib.h>
void t0(void *n)
{
	int i;
	for(i=0;i<10;i++)
		printf("%d ",i);
	printf("\n");
}
int main ()
{
  int count =2;
  MyThreadInit(t0, 0);
  //hmm(c);
  printf("Exiting in main thread, Bye :)\n");
  return 0;
}