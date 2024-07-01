#include <stdio.h>
#include <stdlib.h>

int sum(int val, int inc)
{
	return val + inc;
}

int mult(int val, int fact) 
{
	return val * fact;
}

void mod(int* num, int modifier, int (*modfunc)(int, int))
{
	printf("%p\n", modfunc);
	*num = modfunc(*num, modifier);
}

int main()
{
	int x = 10;
	int modifier = 8;
	// 
	// mod(&x, modifier, &mult);
	// printf("%d\n", x);
	
	// /* x = 5 -> Fallo de segmentación, no se puede modificar el valor de una variable constante*/
	// printf("%p\n", xptr);
	// xptr = &j;
	// printf("%p\n", xptr);;

	// void* nllptr = NULL;
	//
	// char name[5] = {'H', 'e', 'n', 'l', 'o'};
	// char* nameptr = name;
	// char** nameptrptr = &nameptr;
	// char*** name3ptr = &nameptrptr;
	// printf("%c\n", ***name3ptr);
}
