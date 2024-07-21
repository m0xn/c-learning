#include <stdio.h>

#define ARR_SIZE 10

void rm_element(int *arr, size_t *el_count, int element) {
	if (*el_count == 0) {
		fprintf(stderr, "The array is empty\n");
		return;
	}

	size_t i;

	for (i = 0; arr[i] != element && i < *el_count-1; ++i);

	if (i == *el_count-1) {
		fprintf(stderr, "element %d is not in the array\n", element);
		return;
	}

	for (size_t j = i; j < *el_count-1; ++j)
		arr[j] = arr[j+1];

	arr[(*el_count)--] = 0;
}

int main()
{
	int nums[] = {1, 2, 3, 4, 5};
	size_t nums_count = 5;
	
	for (size_t i = 0; i < nums_count; ++i)
		printf("%d\n", nums[i]);

	rm_element(nums, &nums_count, 2);
	rm_element(nums, &nums_count, 1);
	rm_element(nums, &nums_count, 3);
	printf("After algo\n");

	for (size_t i = 0; i < nums_count; ++i)
		printf("%d\n", nums[i]);
}
