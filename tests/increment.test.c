#include <stdio.h>

#define MAX_CAP 1000

int getLine(char s[], int maxlength);

int main() 
{
	int n = 5, x;

	x = n++; /* x = n -> n = n + 1 */
	printf("%d, %d\n", x, n);

	x = ++n; /* n = n + 1 -> x = n */
	printf("%d %d\n", x, n);
}

int getLine(char s[], int maxlength) {
	
}
