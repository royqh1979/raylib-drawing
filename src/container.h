#ifndef CONTAINERS_H
#define CONTAINERS_H
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define QUEUE_BLOCK_SIZE 65536
typedef struct _IntQueue_Node{
	int datas[QUEUE_BLOCK_SIZE];
	int front_idx;
	int end_idx;
	struct _IntQueue_Node* next;
} IntQueue_Node;

typedef struct {
	IntQueue_Node *head;
	IntQueue_Node *tail;
} IntQueue ;

inline void int_queue_init(IntQueue* queue);
inline void int_queue_push(IntQueue* queue,int data);
inline bool int_queue_empty(IntQueue* queue);
inline int int_queue_pop(IntQueue* queue);
inline void int_queue_clear(IntQueue* queue);
inline IntQueue_Node* int_queue_create_node();

IntQueue_Node* int_queue_create_node() {
	IntQueue_Node* p;
	p=(IntQueue_Node*)malloc(sizeof(IntQueue_Node));
	p->front_idx=0;
	p->end_idx=0;
	p->next=NULL;
	return p;		
} 
void int_queue_init(IntQueue* queue){
	queue->head=NULL;
	queue->tail=NULL;
}

void int_queue_push(IntQueue* queue,int data)  {
	IntQueue_Node* p=queue->tail;
	if (p==NULL) {
		p=int_queue_create_node();
		queue->tail=p;
		queue->head=p;
	} else if (p->end_idx>=QUEUE_BLOCK_SIZE) {
		p=int_queue_create_node();
		queue->tail->next=p;
		queue->tail=p;
	}
	p->datas[p->end_idx]=data;
	p->end_idx++;
}

bool int_queue_empty(IntQueue* queue){
	return queue->head==NULL;
}

int int_queue_pop(IntQueue* queue) {
	IntQueue_Node* p=queue->head;
	if (p!=NULL) {
		if (p->front_idx<p->end_idx) {
			int result = p->datas[p->front_idx];
			p->front_idx++;
			if (p->front_idx>=p->end_idx) {
				queue->head=p->next;
				free(p);
				if (queue->head==NULL)
					queue->tail=NULL;
			} 
			return result;
		} else
			return -2;
	}
	return -1;
}

void int_queue_clear(IntQueue* queue) {
	IntQueue_Node* p=queue->head;	
	while (p!=NULL) {
		IntQueue_Node* q=p;
		p=p->next;
		free(q);
	}
	queue->tail=NULL;
	queue->head=NULL;
}
#endif
