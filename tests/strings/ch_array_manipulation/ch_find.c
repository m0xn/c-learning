#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char *msg = (char*)calloc(64, sizeof(char));
	strcpy(msg, "Something right here");
	int ch;

	printf("Enter a character you want to search for occurences in \"%s\": ", msg);
	scanf("%c", (char*) &ch);

	size_t count = 1;
	char *occurence = memchr(msg, ch, count);
	do {
		if (!occurence) {
			++count;
			occurence = memchr(msg, ch, count);
			continue;
		}
		printf("Found occurence for '%c' starting \"%s\"\n", ch, occurence);
		break;
	} while(count < strlen(msg));

	if (count == strlen(msg))
		printf("Couldn't find any occurences for '%c' in \"%s\"\n", ch, msg);

	return 0;
}
