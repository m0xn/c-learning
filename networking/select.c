#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define STDIN 0

int main()
{
	struct timeval tv;
	fd_set readfds;

	tv.tv_sec = 2;
	tv.tv_usec = 500000;

	FD_ZERO(&readfds);
	FD_SET(STDIN, &readfds);

	// Don't care about writefds and exceptfds
	select(STDIN+1, &readfds, NULL, NULL, &tv);

	if (FD_ISSET(STDIN, &readfds))
		printf("A key was pressed\n");
	else
		printf("Reached timeout\n");

	return 0;
}
