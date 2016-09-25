#include <stdio.h>
#include<stdlib.h>
#include<ucontext.h>
#include "libtest.h"
#define MEM 8000
#define MAX_THREADS 100

static int currentThread = -1;
static int inThread = 0;
static int numThread = 1;
static ucontext_t orig;
typedef struct{
	ucontext_t context;//context of thread
	//int child[MAX_THREADS];//count of children  
	int id;
}_MyThread;
struct Node {
	_MyThread data;
	struct Node* next;
};

_MyThread running;
//*** Linked List queue*************
struct Node* R_front = NULL;
struct Node* R_rear = NULL;
void Enqueue(_MyThread x) {
	struct Node* temp = 
		(struct Node*)malloc(sizeof(struct Node));
	temp->data =x; 
	temp->next = NULL;
	if(R_front == NULL && R_rear == NULL){
		R_front = R_rear = temp;
		return;
	}
	R_rear->next = temp;
	R_rear = temp;
}

void Dequeue() {
	struct Node* temp = R_front;
	if(R_front == NULL) {
		printf("Queue is Empty\n");
		return;
	}
	if(R_front == R_rear) {
		R_front = R_rear = NULL;
	}
	else {
		R_front = R_front->next;
	}
	free(temp);
}
void Print() {
	struct Node* temp = R_front;
	while(temp != NULL) {
		printf("%d ",(temp->data).id);
		temp = temp->next;
	}
	printf("\n");
} 
/******************************/
void MyThreadInit(void(*start_funct)(void *), void *args){
	//printf("%d\n",*args);
	static ucontext_t mainContext;
	

	getcontext(&orig);
	getcontext(&mainContext);
	mainContext.uc_link=&orig;
	mainContext.uc_stack.ss_sp=malloc(MEM);
	mainContext.uc_stack.ss_size=MEM;
	mainContext.uc_stack.ss_flags=0;
	//makecontext(&mainContext,start_funct,1,args);
	makecontext(&mainContext,(void(*)(void))start_funct,0);
	running.context=mainContext;
	running.id=numThread++;
	printf("Thread ID %d initalising (Main) \n",running.id);
	swapcontext(&orig,&mainContext);
	
}
MyThread MyThreadCreate(void(*start_funct)(void *), void *args){
	_MyThread t;
	getcontext(&t.context);
	t.context.uc_link=0;
	t.context.uc_stack.ss_sp=malloc(MEM);
	t.context.uc_stack.ss_size=MEM;
	t.context.uc_stack.ss_flags=0;
	//makecontext(&(readyqueue[numThread].context),start_funct,1,args);
	makecontext(&t.context,start_funct,1,args);
	t.id=numThread++;	
	printf("Thread ID %d created \n",t.id);
	Enqueue(t);
	Print();
	return *((MyThread*)&t);
}
void MyThreadYield(){
	getcontext(&running.context);
	Enqueue(running);
	printf("Thread ID %d yielding \n",running.id);
	Print();
	if(R_front!=NULL)
		running=R_front->data;
	Dequeue();
	printf("Thread ID %d running \n",running.id);
	swapcontext(&(R_rear->data).context,&running.context);
}
void MyThreadExit(){
	printf("Thread ID %d Exiting \n",running.id);
	Print();
	//if(R_front!=NULL)
	running=R_front->data;
	Dequeue();
	printf("Thread ID %d running \n",running.id);
	//if(running.id==1)
	//	setcontext(&orig);
	setcontext(&running.context);
	
}
void hmm(huh t)
{
	char ch='a';
	t=&ch;
	printf("%c\n",*((char*)t));
}
