#include <stdio.h>
#define ERROR(...) fprintf(stderr, "%s\n", ##__VA_ARGS__)

int main()
{
	ERROR("There", "is", "something", "wrong", "here");
}
