#include "libtest.h"
#include<stdio.h>
#include<stdlib.h>
int a;
MySemaphore s;
void t2(void * l)
{
  ++a;
  printf("Hello from child! Tracker is %d\n",a);
 // MyThreadYield();
  MySemaphoreDestroy(s);
  MyThreadExit();
}
void t1(void * who)
{
  ++a;
  printf("Hello from child! Tracker is %d\n",a);
 // MyThreadYield();
  s=MySemaphoreInit(5);
  MyThreadCreate(t2, (void *)1);
  MyThreadExit();
}
void t0(void * dummy)
{
  a++;	
  printf("Hello from parent! Tracker is %d\n",a);
  MyThread t=MyThreadCreate(t1, (void *)1);
  //MyThreadCreate(t1, (void *)1);
  //MyThreadJoinAll();
  //printf("Wopp de do=%d\n",ts);
  MyThreadExit();
}
int main ()
{
  int count =2;
  MyThreadInit(t0, 0);
  //hmm(c);
  printf("Exiting in main thread, Bye :)\n");
  return 0;
}