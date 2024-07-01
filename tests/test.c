#include <stdio.h>

void increment(int *restrict a, int step)
{
	*a += step;
}

int main()
{
	int x = 10;
	int *ptr = &x;
	increment(ptr, 2);
	printf("%d\n", x);
}
