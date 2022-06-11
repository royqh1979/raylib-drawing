#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define QUEUE_BLOCK_SIZE 65536

#if defined(__cplusplus)
extern "C" {            // Prevents name mangling of functions
#endif
	
	typedef struct _IntQueueNode{
		int datas[QUEUE_BLOCK_SIZE];
		int front_idx;
		int end_idx;
		struct _IntQueueNode* next;
	} IntQueueNode;
	
	typedef struct {
		IntQueueNode *head;
		IntQueueNode *tail;
	} IntQueue ;
	
	typedef struct  {
		int x1;			// x1 is always <= x2
		int x2;
		int x;			// x at the current scanline
		int min_y;
		int max_y;
		int dx;
		int dy;
		int counter;    // use it to update x
		//	int min_y_buddy; //don't need
		int max_y_buddy; 
	} PolyEdge, *PPolyEdge;
	
	typedef struct {
		int size;
		int capacity;
		PPolyEdge* edges;
	} PolyEdgeHeap;
	
	typedef struct {
		int size;
		int capacity;
		PPolyEdge* edges;		
	} PolyEdgeList;
	
	typedef struct {
		int size;
		int capacity;
		int* edges;		
	} IntList;
	
	void IntQueue_init(IntQueue* queue);
	void IntQueue_free(IntQueue* queue);
	bool IntQueue_empty(IntQueue* queue);
	void IntQueue_push(IntQueue* queue,int data);
	int IntQueue_pop(IntQueue* queue);
	void IntQueue_clear(IntQueue* queue);
	
	void PolyEdgeHeap_init(PolyEdgeHeap* heap, int capacity);
	void PolyEdgeHeap_insert(PolyEdgeHeap* heap, PPolyEdge pEdge);
	bool PolyEdgeHeap_empty(PolyEdgeHeap* heap);
	PPolyEdge PolyEdgeHeap_min(PolyEdgeHeap* heap);
	void PolyEdgeHeap_remove_min(PolyEdgeHeap* heap);
	void PolyEdgeHeap_floatdown(PolyEdgeHeap* heap, int i);
	void PolyEdgeHeap_free(PolyEdgeHeap* heap);
	
	void PolyEdgeList_init(PolyEdgeList* list, int capacity);
	void PolyEdgeList_append(PolyEdgeList* list, PPolyEdge e);
	void PolyEdgeList_remove(PolyEdgeList* list, int i);
	void PolyEdgeList_clear(PolyEdgeList* list);
	void PolyEdgeList_free(PolyEdgeList* list);
	void PolyEdgeList_free_all_nodes(PolyEdgeList* list);
	void PolyEdgeList_sort_by_x(PolyEdgeList* list);
	
	void IntList_init(IntList* list, int capacity);
	void IntList_append(IntList* list, int data);
	void IntList_remove(IntList* list, int i);
	void IntList_clear(IntList* list);
	void IntList_free(IntList* list);
	void IntList_sort(IntList *list);
	
	
#if defined(__cplusplus)
}
#endif
#endif

