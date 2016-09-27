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


struct c_Node {
	int data;
	struct c_Node* next;
};
void add_child(int x, struct c_Node** list){
	struct c_Node* temp=(struct c_Node*)malloc(sizeof(struct c_Node));
	temp->data=x;
		
	if(*list==NULL)
	{	*list=temp; (*list)->next=NULL;return;}
	temp->next=*list;
	*list=temp;
	

}
int c_search(int id,struct c_Node* list){
	struct c_Node* ptr=list;
	while(ptr!=NULL)
	{	
		if(id==ptr->data)
			return 1;
		ptr=ptr->next;
	}
	return 0;
}
void print(struct c_Node* list){
	struct c_Node* ptr=list;
	while(ptr!=NULL)
	{	printf("%d ",ptr->data);ptr=ptr->next;}
	printf("\n");
}
void rm_child(int id,struct c_Node** list){
	struct c_Node* ptr=*list;
	struct c_Node* prev;
	if(ptr!=NULL && ptr->data==id){
		*list=ptr->next;
		free(ptr);
		return;
	}
	while(ptr!=NULL && ptr->data!=id )
	{
		prev=ptr;
		ptr=ptr->next;
	}
	if(ptr==NULL) return;
	prev->next=ptr->next;
	free(ptr);
}

void main()
{
    struct c_Node* clist;
	add_child(1,&clist);
	add_child(3,&clist);
	add_child(5,&clist);
	add_child(7,&clist);
	
	//printf("%d\n",c_search(4,clist));
	//print(clist);
	print(clist);
	rm_child(3,&clist);
	print(clist);
	rm_child(1,&clist);
	print(clist);
	rm_child(7,&clist);
	print(clist);
	
}