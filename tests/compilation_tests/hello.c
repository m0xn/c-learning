#include <stdio.h>
#include <stdint.h>

#define TODO printf("%s:%d -> \"%s()\" no está implementada todavía\n", __FILE__, __LINE__,  __func__)
#define PR(...) printf("%s", #__VA_ARGS__)

int main()
{
	PR(1, x, ñlkjasdfkljasdf, int, sizeof, 1230.21, __FILE__, \n);
// #ifdef AREA 
// 	printf("EL valor de área es: %d\n", AREA(10, 5));
// #else
// 	printf("La macro LIMIT no está definida\n");
// #endif
}
