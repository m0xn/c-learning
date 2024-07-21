#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	const char *msg1 = "This is a test message";
	const char *msg2 = "This is another message";
	char *msg3 = strdup(msg1);

	printf("msg1 and msg2 %s\n", strcmp(msg1, msg2) == 0 ? "Are equal!" : "Do not match...");
	printf("msg1 and msg3 %s\n", strcmp(msg1, msg3) == 0 ? "Are equal!" : "Do not match...");
	printf("msg2 and msg3 %s\n", strcmp(msg2, msg3) == 0 ? "Are equal!" : "Do not match...");

	free(msg3);
	return 0;
}

// The same thing can be done with the `strcoll()` function
// the only difference is that strcoll takes in account the
// system locale, which ultimately refers to system's language
// parameters
