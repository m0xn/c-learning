#include <stdio.h>

void xor_swap(int *a, int *b)
{
	*a = (*a ^ *b) ^ *a;
	*b = (*a ^ *b) ^ *b;
}

int main()
{
	int a = 10, b = 20, c;
	printf("Antes del cambio: %d, %d\n", a, b);
	xor_swap(&a, &b);
	printf("Después del cambio: %d, %d", a, b);
}
