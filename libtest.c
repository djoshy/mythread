#include <stdio.h>
#include<stdlib.h>
#include<ucontext.h>
#include "libtest.h"
#define MEM 8000
#define MAX_THREADS 100
static _MyThread readyqueue[MAX_THREADS];
static int currentThread = -1;
static int inThread = 0;
static int numThread = 0;
static ucontext_t mainContext;
static ucontext_t orig;

typedef struct{
	ucontext_t context;//context of thread
	int active;//1 if active, 0 if not
	int child[MAX_THREADS];//count of children  
	
}_MyThread;
int unpack(void *params)
{
	int k=1;
	//sscanf(params,"%d",&k);
	printf("%d",k);
	return k;
}
void MyThreadInit(void(*start_funct)(void *), void *args){
	//printf("%d\n",*args);
	getcontext(&orig);
	getcontext(&mainContext);
	mainContext.uc_link=&orig;
	mainContext.uc_stack.ss_sp=malloc(MEM);
	mainContext.uc_stack.ss_size=MEM;
	mainContext.uc_stack.ss_flags=0;
	//unpack(args);
	//int k=10;
	makecontext(&mainContext,(void(*)(void))start_funct,0);
	swapcontext(&orig,&mainContext);
	
}
MyThread MyThreadCreate(void(*start_funct)(void *), void *args){
	readyqueue[numThread].context.uc_link=0;
	readyqueue[numThread].context.uc_link=&orig;
	readyqueue[numThread].context.uc_stack.ss_sp=malloc(MEM);
	readyqueue[numThread].context.uc_stack.ss_size=MEM;
	readyqueue[numThread].context.uc_stack.ss_flags=0;
	//readyqueue[numThread]
}
void hmm(huh t)
{
	char ch='a';
	t=&ch;
	printf("%c\n",*((char*)t));
}
