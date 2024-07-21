#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char *msg1 = (char*)calloc(64, sizeof(char));
	char *msg2 = (char*)calloc(64, sizeof(char));
	char *msg3 = (char*)calloc(64, sizeof(char));

	strcpy(msg1, "Something here");
	strcpy(msg2, "Something there");
	strcpy(msg3, "Something here");

	printf("msg1 and msg2 buffers are: %s\n", memcmp(msg1, msg2, strlen(msg1)) == 0 ? "Exactly equal!" : "Not equal...");
	printf("msg2 and msg3 buffers are: %s\n", memcmp(msg2, msg3, strlen(msg1)) == 0 ? "Exactly equal!" : "Not equal...");
	printf("msg1 and msg3 buffers are: %s\n", memcmp(msg1, msg3, strlen(msg1)) == 0 ? "Exactly equal!" : "Not equal...");

	free(msg1);
	free(msg2);
	free(msg3);

	return 0;
}

// This script does the exact same thing that the ../examination/str_comparison.c
// one, the only thing changing that we're considering C strings as character arrays.
