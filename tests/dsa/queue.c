#include <stdio.h>
#include <stdlib.h>

#define QUEUE_CAP 256 

typedef struct {
	int elements[QUEUE_CAP];
	size_t count;
} Queue;

int is_full(Queue* queue) { return queue->count == QUEUE_CAP; }
int is_empty(Queue* queue) { return queue->count == 0; }

void enqueue(Queue* queue, int element)
{
	if (is_full(queue)) {
		perror("The queue is already full!");
		exit(1);
	}

	queue->count++; /* Make room for the extra element */
	for (size_t i = queue->count-1; i > 0; --i) {
		queue->elements[i] = queue->elements[i-1];
	}

	queue->elements[0] = element;
}

int dequeue(Queue *queue) {
	if (is_empty(queue)) {
		perror("The queue is empty!");
		exit(1);
	}

	int element = queue->elements[queue->count-1];
	queue->elements[queue->count-1] = 0;
	queue->count--;
	return element;
}

int peek(Queue *queue) { return queue->elements[queue->count-1]; }

Queue* init_queue() {
	Queue *queue = (Queue*)malloc(sizeof(Queue));
	queue->count = 0;
	return queue;
}

int main()
{
	Queue *queue = init_queue();
	enqueue(queue, 10);
	enqueue(queue, 20);
	enqueue(queue, 30);
	enqueue(queue, 40);
	printf("The element %d was dequeued.\n", dequeue(queue));
}
