#include <stdio.h>
#include <stdlib.h>

#define CAP 5

int main()
{
	int *arr = calloc(CAP, sizeof(int));

	for (size_t i = 0; i < CAP; ++i)
		arr[i] = i*i;

	int *new_arr = realloc(arr, sizeof(int)*CAP*2);

	for (size_t i = 0; i < CAP*2; ++i)
		printf("Element at index %d: %d\n", i, new_arr[i]);

	free(arr);
	/* free(n) intenta liberar una región de memoria que ha sido ya liberada anteriormente con free(arr) */
}
