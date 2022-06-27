#include "utils.h"

static IntQueueNode* int_queue_create_node() {
	IntQueueNode* p;
	p=(IntQueueNode*)malloc(sizeof(IntQueueNode));
	p->front_idx=0;
	p->end_idx=0;
	p->next=NULL;
	return p;		
} 

void IntQueue_init(IntQueue* queue){
	queue->head=NULL;
	queue->tail=NULL;
}

void IntQueue_free(IntQueue* queue) {
	IntQueue_clear(queue);
}

void IntQueue_push(IntQueue* queue,int data)  {
	IntQueueNode* p=queue->tail;
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

bool IntQueue_empty(IntQueue* queue){
	return queue->head==NULL;
}

int IntQueue_pop(IntQueue* queue) {
	IntQueueNode* p=queue->head;
	//	if (p!=NULL) {
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
	//	}
	//	return -1;
}

void IntQueue_clear(IntQueue* queue) {
	IntQueueNode* p=queue->head;	
	while (p!=NULL) {
		IntQueueNode* q=p;
		p=p->next;
		free(q);
	}
	queue->tail=NULL;
	queue->head=NULL;
}

static int heap_parent(int idx) {
	return idx/2; 
}

static int heap_left(int idx) {
	return idx*2;
}

static int heap_right(int idx) {
	return idx*2+1;
}

void PolyEdgeHeap_init(PolyEdgeHeap* heap, int capacity){
	//	if(capacity<0) 
	//		capacity=0;
	heap->capacity=capacity;
	heap->size=0;
	//	if (heap->capacity>0)
	heap->edges=(PPolyEdge*)malloc(sizeof(PPolyEdge)*(capacity+1));
	//	else
	//		heap->edges=NULL;
}

void PolyEdgeHeap_free(PolyEdgeHeap* heap) {
	if (heap->edges!=NULL)
		free(heap->edges);
	heap->size=0;
	heap->capacity=0;
}

void PolyEdgeHeap_insert(PolyEdgeHeap* heap, PPolyEdge pEdge) {
	//	if (heap->size>=heap->capacity)
	//		return false;
	heap->size++;
	heap->edges[heap->size]=pEdge;
	int i=heap->size;
	int parent=heap_parent(i);
	while(i>1 && heap->edges[parent]->min_y>heap->edges[i]->min_y) {
		PPolyEdge p=heap->edges[parent];
		heap->edges[parent]=heap->edges[i];
		heap->edges[i]=p;
		i=parent;
		parent=heap_parent(i);
	}
}

bool PolyEdgeHeap_empty(PolyEdgeHeap* heap) {
	return heap->size==0;	
}

PPolyEdge PolyEdgeHeap_min(PolyEdgeHeap* heap) {
	//	if (heap->size==0)
	//		return NULL;
	return heap->edges[1];
}

void PolyEdgeHeap_remove_min(PolyEdgeHeap* heap) {
	//	if (heap->size==0)
	//		return;
	heap->edges[1]=heap->edges[heap->size];
	heap->size--;
	PolyEdgeHeap_floatdown(heap,1);
}

void PolyEdgeHeap_floatdown(PolyEdgeHeap* heap, int i) {
	int l = heap_left(i);
	int r = heap_right(i);
	int smallest;
	if (l<=heap->size && heap->edges[l]->min_y<heap->edges[i]->min_y)
		smallest=l;
	else
		smallest=i;
	if (r<=heap->size && heap->edges[r]->min_y<heap->edges[smallest]->min_y)
		smallest=r;
	if (smallest!=i) {
		PPolyEdge p=heap->edges[smallest];
		heap->edges[smallest]=heap->edges[i];
		heap->edges[i]=p;
		PolyEdgeHeap_floatdown(heap,smallest);
	}
}

void PolyEdgeList_init(PolyEdgeList* list, int capacity) {
	list->capacity=capacity;
	list->size=0;
	//	if (capacity>0)
	list->edges=(PPolyEdge*)malloc(sizeof(PPolyEdge)*capacity);
	//	else
	//		list->edges=NULL;
}

void PolyEdgeList_append(PolyEdgeList* list, PPolyEdge e) {
	//	if (list->size>=list->capacity)
	//		return false;
	list->edges[list->size]=e;
	list->size++;
}
void PolyEdgeList_remove(PolyEdgeList* list, int i) {
	//	if (list->size==0)
	//		return;
	//	if (i>=list->size)
	//		return;
	list->edges[i]=list->edges[list->size-1];
	list->size--;
}
void PolyEdgeList_clear(PolyEdgeList* list) {
	list->size=0;
}
void PolyEdgeList_free(PolyEdgeList* list) {
	list->size=0;
	list->capacity=0;
	free(list->edges);
	list->edges=NULL;
}

void PolyEdgeList_free_all_nodes(PolyEdgeList* list){
	for (int i=0;i<list->capacity;i++) {
		free(list->edges[i]);
	}
}

static int _compare_polyedge_by_x(const void * p1, const void * p2) {
	PPolyEdge* pp1=(PPolyEdge*)p1;
	PPolyEdge* pp2=(PPolyEdge*)p2;
	return (*pp1)->x-(*pp2)->x;
}

void PolyEdgeList_sort_by_x(PolyEdgeList* list){
	qsort(list->edges,list->size,sizeof(PPolyEdge), _compare_polyedge_by_x);
}

void IntList_init(IntList* list, int capacity){
	list->capacity=capacity;
	list->size=0;
	list->datas=(int*)malloc(sizeof(int)*capacity);
}
void IntList_append(IntList* list, int data) {
	if (list->size>=list->capacity) {
		int capacity;
		if (list->capacity<1024)
			capacity = list->capacity*16;
		else if (list->capacity<1024*1024)
			capacity = list->capacity*4;
		else 
			capacity = list->capacity*2;
		int* newdatas = (int*)realloc(list->datas,sizeof(int)*capacity);
		if (newdatas==NULL) {
			exit(-20);
			return;
		}
		list->datas=newdatas;
		list->capacity=capacity;
	}
	list->datas[list->size]=data;
	list->size++;	
}
void IntList_remove(IntList* list, int i) {
	list->datas[i]=list->datas[list->size-1];
	list->size--;
}
void IntList_clear(IntList* list){
	list->size=0;	
}
void IntList_free(IntList* list){
	list->size=0;
	list->capacity=0;
	free(list->datas);
	list->datas=NULL;
}

static int _compare_int(const void * p1, const void * p2) {
	int* pp1=(int*)p1;
	int* pp2=(int*)p2;
	return (*pp1)-(*pp2);
}

void IntList_sort(IntList *list){
	qsort(list->datas,list->size,sizeof(int), _compare_int);	
}
