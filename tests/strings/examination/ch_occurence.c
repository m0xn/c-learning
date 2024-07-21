#include <stdio.h>
#include <string.h>

int main()
{
	const char *msg = "Here is some kind of message";
	int ch = 'o';
	printf("The first occurence of %c starts at: %s\n", ch, strchr(msg, ch));
	printf("The last occurence of %c starts at: %s\n", ch, strrchr(msg, ch));
}
