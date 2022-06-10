#include <stdio.h>
#include "../src/utils.h"

int main() {
	IntQueue queue;
	int_queue_init(&queue);
	printf("Testing push...:");
	for (int i=0;i<100000;i++) {
		int_queue_push(&queue,i);
	}
	printf("done.\n");
	int count=0;
	printf("Testing popping...:");
	while (!int_queue_empty(&queue)) {
		int d=int_queue_pop(&queue);
		if (d!=count)  {
			printf("failed!,%d %d\n",d,count);
			break;
		}
		count++;
	}
	printf("done.\n");
	printf("Testing pushing after clear...:");
	int_queue_clear(&queue);
	for (int i=0;i<100000;i++) {
		int_queue_push(&queue,100000+i);
	}
	printf("done.\n");
	printf("Testing popping after clear...:");
	while (!int_queue_empty(&queue)) {
		int d=int_queue_pop(&queue);
		if (d!=count)  {
			printf("failed!,%d %d\n",d,count);
			break;
		}
		count++;
	}
	printf("done.\n");
	int_queue_clear(&queue);
	return 0;
}
