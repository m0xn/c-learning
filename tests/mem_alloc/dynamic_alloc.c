#include <stdio.h>
#include <stdlib.h>

/* Program to dyanmically allocate numbers into an array */
/* Idea form https://www.geeksforgeeks.org/dynamic-memory-allocation-in-c-using-malloc-calloc-free-and-realloc/ */
int main()
{
	int index = 0, ans;
	int *arr = (int*)malloc(sizeof(int));

	if (arr == NULL) {
		printf("[ERROR]: couldn't allocate array\n");
		exit(1);
	}

	printf("Introduce the number for the first element: ");
	scanf("%d", &arr[index++]);
	
	do {
		printf("Do you want to allocate more memory? (1/0): ");
		scanf("%d", &ans);

		if (ans == 0) {
			break;
		}

		arr = realloc(arr, sizeof(int)*(index+1));

		if (arr == NULL) {
			printf("[ERROR]: Couldn't allocate more memory");
			exit(1);
		}

		printf("Introduce the next number on the array: ");
		scanf("%d", &arr[index++]);

		printf("Successfully introduced %d at %d\n", arr[index-1], index-1);
	} while (ans == 1);

	printf("Here is the resulting array:\n");
	for (size_t i = 0; i < index; ++i)
		printf("%d, ", arr[i]);
	printf("\n");
	free(arr);
}
