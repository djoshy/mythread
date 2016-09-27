#include "libtest.h"
#include<stdio.h>
#include<stdlib.h>
int a;
MySemaphore s;
void t2(void * l)
{
  ++a;
 // MySemaphoreWait(s);
  printf("Tracker is %d\n",a);
 // MySemaphoreSignal(s);
  
  
  MyThreadExit();
}
void t1(void * who)
{
  
 
 // MyThreadYield();
//  MySemaphoreWait(s);
 // MyThreadCreate(t2, (void *)1);
  ++a;
  printf("Tracker is %d\n",a);
 // MySemaphoreSignal(s);
  
  MyThreadExit();
}
void t0(void * dummy)
{
  a++;	
  printf("Tracker is %d\n",a);
  MyThread t=MyThreadCreate(t1, (void *)1);
  MyThreadCreate(t2, (void *)1);
  MyThreadJoinAll();
  s=MySemaphoreInit(0);
  // MyThreadYield();
  MySemaphoreSignal(s);
  
  MyThreadExit();
}
int main ()
{
  int count =2;
  MyThreadInit(t0, 0);
  //hmm(c);
  printf("Exiting in main context, Bye :)\n");
  return 0;
}