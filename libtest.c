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
	//int child[MAX_THREADS];//ID of children
	int block;//ID of blocking thread. 
	int allblock;
	struct c_Node* clist;
	int id;
	struct _MyThread* parent;//pointer to parent
};
struct c_Node {
	int data;
	struct c_Node* next;
};
struct Node {
	struct _MyThread* data;
	struct Node* next;
};
struct _MyThread *running;
struct _MySemaphore{
	int value;//semaphore value
	//blocked linked list values
	struct Node* f_list;
	struct Node* r_list;
};
//*** Linked List queue*************
struct Node* R_front = NULL;
struct Node* R_rear = NULL;
void add_child(int x, struct Node** list){
	struct c_Node* temp=(struct c_Node*)malloc(sizeof(struct c_Node));
	temp->data=x;
		
	if(*list==NULL)
	{	*list=temp; *list->next=NULL;return;}
	temp->next=*list;
	*list=temp;
	

}
void Enqueue(struct _MyThread *x) {
	//printf("Queue is being added\n");
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
		printf("%d(%d) ",(temp->data)->id,((temp->data)->parent)->id);
		temp = temp->next;
	}
	printf("\n");
} 
/******************************/

void s_add(struct Node** front, struct Node** rear){
	struct Node* temp = 
		(struct Node*)malloc(sizeof(struct Node));
	temp->data =running; 
	temp->next = NULL;
	
	if(*front==NULL && *rear == NULL){ 
		*front=*rear=temp;
		return;
	}
	(*rear)->next = temp;
	(*rear)=temp;
}
void s_remove(struct Node** front, struct Node** rear){
	//struct Node* temp = front;
	if(*front==NULL){
		printf("Semaphore queue is Empty\n");
		return;
	}	
	if(*front==*rear)
		{Enqueue((*front)->data);*front=*rear=NULL;}
	else{
		Enqueue((*front)->data);
		*front=(*front)->next;
	}
	
}


void addChildList(int id){
	int k;
	for(k=0;k<MAX_THREADS;k++)
		if(running->child[k]==0)
		{	running->child[k]=id; break;}
	printf("Thread ID %d added to Parent(%d) child list\nChild list of Thread %d: ",id,running->id,running->id);
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
			for(k=pos;k<MAX_THREADS-1;k++)
				t->child[k]=t->child[k+1];
			break;
		}
	
	printf("Thread %d ID removed from Parent(%d) child list\nChild list: ",id,t->id);
	for(k=0;k<10;k++)
		printf("%d ",t->child[k]);
	printf("\n");
	
}
int checkparChild(int id){
	int k;
	for(k=0;k<MAX_THREADS;k++)
		if(running->parent->child[k]==id)
		{	return 1;}
		
	return 0;		
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
	makecontext(&mainContext,(void *)start_funct,1,args);
	running=malloc(sizeof(struct _MyThread));
	running->context=mainContext;
	running->id=numThread++;
	/*
	int k;
	for(k=0;k<MAX_THREADS;k++){
		running->child[k]=0;
		
	}
	*/
	running->clist==NULL;
	running->block=0;
	running->allblock=0;
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
	makecontext(&t->context,(void *)start_funct,1,args);
	t->id=numThread++;	
	//printf("huuuuh\n");
	/*int k;
	
	for(k=0;k<MAX_THREADS;k++){
		t->child[k]=0;
	}
	*/
	t->clist=NULL;
	t->block=0;
	t->allblock=0;
	t->parent=running;
	printf("Thread ID %d created, ",t->id);
	
	addChildList(t->id);
	//printf("My Parent is %d \n",t->parent->id);
	Enqueue(t);
	Print();
	return ((void*)t);
}
void MyThreadYield(){
	getcontext(&running->context);
	Enqueue(running);
	printf("Thread ID %d yielding\n",running->id);
	running=R_front->data;
	Print();
	Dequeue();
	printf("Thread ID %d running\n",running->id);
	//printf("My Parent is %d \n",running->parent->id);
	if(R_rear!=NULL)
	swapcontext(&((R_rear->data)->context),&running->context);
}
int MyThreadJoin(MyThread thread){
	struct _MyThread* t;
	t=(struct _MyThread*)(thread);
	if(checkChild(t->id)!=0)
	{	
		printf("Thread ID %d waiting for Thread ID %d\n",running->id,t->id);
		running=R_front->data;
		t->parent->block=t->id;
		Print();
		Dequeue();
		printf("Thread ID %d running(from Join)\n",running->id);
		swapcontext(&((t->parent)->context),&running->context);
		return 0;
	}
	else
		return -1;
	
}
void MyThreadExit(){
	printf("Thread ID %d Exiting, ",running->id);
	Print();
	remList(running->id);
	if(running->parent->allblock==1){// || checkparChild(running->id)==1){//check for parent having JoinAll on
		if(running->parent->child[0]==0)
		{	printf("Finishing Join all for %d\n",running->parent->id);Enqueue(running->parent);running->parent->allblock=0;}
	}
	
	if(running->parent->block==running->id)//check for parent having Join on
	{	Enqueue(running->parent); running->block=0;}
	if(R_front==NULL )//check for ready queue being empty
		setcontext(&orig);
	running=R_front->data;
	Dequeue();
	printf("Thread ID %d running(from Exit) \n",running->id);
	
	setcontext(&running->context);
}
void MyThreadJoinAll()
{
	if(running->child[0]==0)
		return;
	struct _MyThread* t=malloc(sizeof(struct _MyThread));
	t=running;
	running->allblock=1;
	running=R_front->data;
	printf("Thread ID %d waiting for children\n",t->id);
	printf("Thread ID %d running(from Joinall)\n",running->id);
	Dequeue();
	swapcontext(&(t->context),&running->context);
}
MySemaphore MySemaphoreInit(int initialValue)
{
	if(initialValue<0)
		return NULL;
	struct _MySemaphore *s=malloc(sizeof(struct _MySemaphore));
	s->value=initialValue;
	s->f_list=s->r_list=NULL;
	printf("Semaphore with value %d created by Thread %d\n",s->value,running->id);
	return (void*)s;
}
int MySemaphoreDestroy(MySemaphore sem){
	struct _MySemaphore *s=(struct _MySemaphore*)sem;
	if(s->f_list==NULL)
	{	printf("Semaphore with value %d destroyed by Thread %d\n",s->value,running->id);free(s); return 0;
	}
	else
	{	printf("Semaphore with value %d cannot be destroyed!!!\n",s->value);return -1; 
	}
}
void MySemaphoreWait(MySemaphore sem){
	printf("Thread ID %d is waiting on semaphore\n",running->id);
	struct _MySemaphore *s=(struct _MySemaphore*)sem;
	if(s->value<1){
	s_add(&(s->f_list),&(s->r_list));
	running=R_front->data;
	Dequeue();
	printf("Added Thread %d to sem's blocking queue\n",s->r_list->data->id);
	printf("Thread ID %d running(from semwait)\n",running->id);
	swapcontext(&((s->r_list->data)->context),&running->context);
	}
	else
		(s->value)--;
		
}
void MySemaphoreSignal(MySemaphore sem){
	struct _MySemaphore *s=(struct _MySemaphore*)sem;
	if(s->f_list!=NULL){
		printf("Added Thread %d to ready queue(from semsig)\n",s->f_list->data->id);
		s_remove(&(s->f_list),&(s->r_list));
		Print();
	}
	else
		(s->value)++;
}