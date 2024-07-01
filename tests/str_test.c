#include <stdio.h>
#include <string.h>

int main()
{
	const char *sample = "// This is a C commment";
	const char *occ = "//";

	printf("The first ocurrence is: %c\n", *strstr(sample, occ));
}
