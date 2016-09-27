#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>
/******************************************************************************
 *
 *  File Name........: fib.c
 *
 *  Description......:
 *
 *  Created by vin on 11/21/06
 *
 *  No warranty!  This program has not been compiled or tested because
 *  I do not have a library with which to do so.
 *
 *  It is provided for illusration.  
 *
 *****************************************************************************/
#include "libtest.h"


#include <stdio.h>

MySemaphore a;

void t1(void * dummy)
{
  printf("t1 start\n");
  MyThreadExit();
}
void t2(void * dummy)
{
  MySemaphoreWait(a);
  printf("t2 start\n");

  MyThreadExit();
}

void t0(void * dummy)
{
  a= MySemaphoreInit(0);
  printf("t0 start\n");
  MyThreadCreate (t2, NULL);
  MyThreadCreate (t2, NULL);
  
  
  //MyThread ptr=MyThreadCreate (t1, NULL);
  //MyThreadCreate (t1, NULL);
  MyThreadYield();
  printf(" \n %d ",MySemaphoreDestroy(a));
  MySemaphoreSignal(a);
  MyThreadYield();
  MySemaphoreSignal(a);
  //printf(" \n %d ",MyThreadJoin(ptr));
 // printf(" \n %d ",MySemaphoreDestroy(a));
   
  MyThreadExit();
}



int main()
{
  MyThreadInit(t0, NULL);
}

