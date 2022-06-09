#include <stdio.h>
#include "../src/container.h"

int main() {
	IntQueue queue;
	int_queue_init(&queue);
	for (int i=0;i<100000;i++) {
		int_queue_push(&queue,i);
	}
	int count=0;
	while (!int_queue_empty(&queue)) {
		int d=int_queue_pop(&queue);
		if (d!=count)  {
			printf("failed!,%d %d\n",d,count);
			break;
		}
		count++;
	}
	printf("????\n");
	int_queue_clear(&queue);
	for (int i=0;i<100000;i++) {
		int_queue_push(&queue,100000+i);
	}
	while (!int_queue_empty(&queue)) {
		int d=int_queue_pop(&queue);
		if (d!=count)  {
			printf("failed!,%d %d\n",d,count);
			break;
		}
		count++;
	}
	return 0;
}
