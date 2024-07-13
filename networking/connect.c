#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

int main()
{
	int status;
	const char *host = "www.google.com";
	const char *port = "3409";
	struct addrinfo hints;
	struct addrinfo *res;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // Tells to use the local machine's IP

	if ((status = getaddrinfo(host, port, &hints, &res)) != 0) {
		printf("[ERROR] getaddrinfo: %s\n", gai_strerror(status));
		exit(1);
	}

	int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (socket_fd == -1) {
		fprintf(stderr, "[ERROR] socket couldn't be created");
		exit(1);
	}

	// Bind needn't be called in this case becasue we only care about the remote
	// host we're trying to connect to. The kernel will do the heavy lifting of
	// binding the connection to an unused port
	
	if (connect(socket_fd, res->ai_addr, res->ai_addrlen) != 0) {
		fprintf(stderr, "Couldn't connect %s on port %s", host, port);
		exit(1);
	}

	freeaddrinfo(res);
	return 0;
}

