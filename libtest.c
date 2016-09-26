#include <stdio.h>
#include<stdlib.h>
#include<ucontext.h>
#include "libtest.h"
#define MEM 8000
#define MAX_THREADS 250

static int currentThread = -1;
static int inThread = 0;
static int numThread = 1;
static ucontext_t orig;
struct _MyThread{
	ucontext_t context;//context of thread
	int child[MAX_THREADS];//ID of children
	int block;//ID of blocking thread. 	
	int id;
	struct _MyThread* parent;//pointer to parent
};
struct Node {
	struct _MyThread* data;
	struct Node* next;
};

struct _MyThread *running;
//*** Linked List queue*************
struct Node* R_front = NULL;
struct Node* R_rear = NULL;
void Enqueue(struct _MyThread *x) {
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
	printf("Ready queue: ");
	while(temp != NULL) {
		//printf("%d ",(temp->data).id);
		printf("%d(%d)",(temp->data)->id,((temp->data)->parent)->id);
		temp = temp->next;
	}
	printf("\n");
} 
/******************************/

void addChildList(int id){
	int k;
	for(k=0;k<MAX_THREADS;k++)
		if(running->child[k]==0)
		{	running->child[k]=id; break;}
	printf("Thread %d ID added to Parent(%d) child list\nChild list: ",id,running->id);
	for(k=0;k<10;k++)
		printf("%d ",running->child[k]);
	printf("\n");

}
int checkChild(int id){
	int k;
	for(k=0;k<MAX_THREADS;k++)
		if(running->child[k]==id||running->child[k]==0)
			return running->child[k];
}
void remList(int id){
	struct _MyThread* t=running->parent;
	int k,pos;
	for(k=0;k<MAX_THREADS;k++)
		if(t->child[k]==id)
		{	pos=k;
			printf("FOUND AT%d\n",pos);
			for(k=pos;k<MAX_THREADS-1;k++)
				t->child[k]=t->child[k+1];
			break;
		}
	
	printf("Thread %d ID removed from Parent(%d) child list\nChild list: ",id,t->id);
	for(k=0;k<10;k++)
		printf("%d ",t->child[k]);
	printf("\n");
	
}
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
	running=malloc(sizeof(struct _MyThread));
	running->context=mainContext;
	running->id=numThread++;
	
	int k;
	for(k=0;k<MAX_THREADS;k++){
		running->child[k]=0;
		running->block=0;
	}
	running->parent=running;
	printf("Thread ID %d initalising (Main) \n",running->id);
	swapcontext(&orig,&mainContext);
	
}
MyThread MyThreadCreate(void(*start_funct)(void *), void *args){
	struct _MyThread *t;
	t=malloc(sizeof(struct _MyThread));
	getcontext(&t->context);
	t->context.uc_link=0;
	t->context.uc_stack.ss_sp=malloc(MEM);
	t->context.uc_stack.ss_size=MEM;
	t->context.uc_stack.ss_flags=0;
	//makecontext(&(readyqueue[numThread].context),start_funct,1,args);
	makecontext(&t->context,start_funct,1,args);
	t->id=numThread++;	
	
	int k;
	for(k=0;k<MAX_THREADS;k++){
		t->child[k]=0;
		t->block=0;
	}
	t->parent=running;
	addChildList(t->id);
	printf("Thread ID %d created\n",t->id);
	//printf("My Parent is %d \n",t->parent->id);
	Enqueue(t);
	Print();
	printf("%x\n",t);
	return *((MyThread*)t);
}
void MyThreadYield(){
	getcontext(&running->context);
	Enqueue(running);
	printf("Thread ID %d yielding\n",running->id);
	Print();
	running=R_front->data;
	Print();
	Dequeue();
	printf("Thread ID %d running, ",running->id);
	//printf("My Parent is %d \n",running->parent->id);
	if(R_rear!=NULL)
	swapcontext(&((R_rear->data)->context),&running->context);
}
int MyThreadJoin(MyThread thread){
	struct _MyThread *t;
	printf("%x\n",thread);
	//t=malloc(sizeof(struct _MyThread));
	t=(struct _MyThread*)(&thread);
	//printf("%d\n",t->id);
	if(checkChild(t->id)!=0)
		printf("Child owoo\n");
	
}
void MyThreadExit(){
	printf("Thread ID %d Exiting, ",running->id);
	
	printf("My Parent is %d \n",running->parent->id);
	Print();
	//if(R_front!=NULL)
	remList(running->id);
	running=R_front->data;
	Dequeue();
	printf("Thread ID %d running \n",running->id);
	//if(running.id==1)
	//	setcontext(&orig);
	setcontext(&running->context);
	
}

