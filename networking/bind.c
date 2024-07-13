#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

int main()
{
	int status;
	struct addrinfo hints;
	struct addrinfo *res;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // Tells to use the local machine's IP

	if ((status = getaddrinfo(NULL, "3409", &hints, &res)) != 0) {
		printf("[ERROR] getaddrinfo: %s\n", gai_strerror(status));
		exit(1);
	}

	int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (socket_fd == -1) {
		fprintf(stderr, "[ERROR] socket couldn't be created");
		exit(1);
	}

	bind(socket_fd, res->ai_addr, res->ai_addrlen);

	// You should use ports from 1024 on above until 65535 for your application
	// To avoid getting "Adress already in use" error message, caused due to a
	// bit of a socket being left in the kernel, use the following code:

	int yes = 1;
	if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) != 0) {
		fprintf(stderr, "[ERROR] Couldn't reuse address");
		exit(1);
	}

	freeaddrinfo(res);
	return 0;
}
