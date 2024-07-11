#include <stdio.h>
#include <stdlib.h>

#define CAP 5

int main()
{
	int *arr = calloc(CAP, sizeof(int));
	
	for (size_t i = 0; i < CAP; ++i) {
		arr[i] = i*i;
		printf("Element at index %d: %d\n", i, arr[i]);
	}

	free(arr);
	return 0;
}
