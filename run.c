#include "libtest.h"
#include<stdio.h>
#include<stdlib.h>
void t1(void * who)
{
  
  printf("Hello from child!\n");
 // MyThreadYield();
  
  MyThreadExit();
}

void t0(void * dummy)
{
  MyThread t=MyThreadCreate(t1, (void *)1);
  //MyThreadCreate(t1, (void *)1);
  
  MyThreadJoin(t);
  //printf("huh");
}
int main ()
{
  int count =2;
  MyThreadInit(t0, 0);
  //hmm(c);
  printf("Exiting in main thread, Bye :)\n");
  return 0;
}