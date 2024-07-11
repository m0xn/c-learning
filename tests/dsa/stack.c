#include <stdio.h>
#include <stdlib.h>

#define STACK_CAP 64

typedef struct {
	int elements[STACK_CAP];
	size_t count;
} Stack;

int is_empty(Stack *ref) { return ref->count == 0; }
int is_full(Stack *ref) { return ref->count == STACK_CAP; }

void push(Stack *ref, int element)
{
	if (is_full(ref)) {
		perror("The stack is already full");
		exit(1);
	}

	ref->count++;
	ref->elements[ref->count-1] = element;
}

int pop(Stack *ref)
{
	if (is_empty(ref)) {
		perror("the stack is empty");
		exit(1);
	}

	int element = ref->elements[ref->count-1];
	ref->elements[ref->count-1] = 0;
	ref->count--;
	return element;
}

int peek(Stack *ref)
{
	if (is_empty(ref)) {
		perror("the stack is empty");
		exit(1);
	}

	return ref->elements[ref->count-1];
}

Stack* init_stack()
{
	Stack *instance = (Stack*)malloc(sizeof(Stack));
	instance->count = 0;
	return instance;
}

void print_stack(Stack *ref)
{
	for (size_t i = 0; i < ref->count; ++i)
		printf(i == ref->count-1 ? "%d" : "%d, ", ref->elements[i]);
	printf("\n");
}

int main()
{
	Stack *stack = init_stack();
	push(stack, 10);
	push(stack, 20);
	push(stack, 30);
	push(stack, 40);
	push(stack, 50);
	printf("Popped element from the stack: %d\n", pop(stack));
	printf("Last element of the stack: %d\n", peek(stack));
	print_stack(stack);
}
