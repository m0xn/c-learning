#include <stdio.h>
#include <poll.h>

int main()
{
	struct pollfd pfds[1];

	pfds[0].fd = 0; // Standard input
	pfds[0].events = POLLIN; // Tell me when ready to read
	
	printf("Hit RETURN or wait 2.5 seconds for the timeout\n");

	int num_events = poll(pfds, 1, 2500);

	if (num_events == -1) {
		perror("poll");
		return 1;
	}

	if (num_events == 0) { // A value of 0 indicates timeout on the poll() func
		printf("Poll timed out!\n");
	} else {
		int pollin_happened = pfds[0].revents & POLLIN;

		pollin_happened
			? printf("File descriptor %d is ready to read!\n", pfds[0].fd)
			: printf("Unexpected event ocurred: %d\n", pfds[0].revents);
	}

	return 0;
}
