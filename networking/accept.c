#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>

#define PORT "8080"
#define BACKLOG 5 // Max connections to be queued until accept() 

int main()
{
	int status;
	struct sockaddr_storage connection_addr;
	socklen_t addr_len;
	struct addrinfo hints, *res;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // Tells to use the local machine's IP

	if ((status = getaddrinfo(NULL, PORT, &hints, &res)) != 0) {
		printf("[ERROR] getaddrinfo: %s\n", gai_strerror(status));
		exit(1);
	}

	int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (socket_fd == -1) {
		fprintf(stderr, "[ERROR] socket couldn't be created");
		exit(1);
	}

	bind(socket_fd, res->ai_addr, res->ai_addrlen);

	if (listen(socket_fd, BACKLOG) != 0) {
		fprintf(stderr, "Couldn't start listening...\n");
		exit(1);
	}

	addr_len = sizeof connection_addr;
	
	// This new socket file descriptor will be used for communication
	// with connected clients
	int conn_fd = accept(socket_fd, (struct sockaddr*) &connection_addr, &addr_len);
	if (conn_fd == -1) {
		printf("Couldn't establish a connection with the client");
		exit(1);
	}

	// If you only wanted one client to connect to the server you could
	// close the file descriptor for the first socket after the new one
	// created, that way stopping other machines for trying to connect.

	freeaddrinfo(res);
	return 0;
}
