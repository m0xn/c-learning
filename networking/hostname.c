#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>

#define LISTENING_PORT "3000"
#define BUFF_SIZE 1024

int main()
{
	int status;
	struct addrinfo hints, *res;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	status = getaddrinfo(NULL, LISTENING_PORT, &hints, &res);
	if (status < 0) {
		fprintf(stderr, "[ERROR] Failed to get addrinfo: %s\n", gai_strerror(status));
		exit(1);
	}

	int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (socket_fd < 0) {
		fprintf(stderr, "[ERROR] Failed to initliaze socket...\n");
		exit(1);
	}

	int yes = '1';
	if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) < 0) {
		fprintf(stderr, "[ERROR] Failed to configure socket to reuse an address");
		exit(1);
	}

	int bind_res = bind(socket_fd, res->ai_addr, res->ai_addrlen);
	if (bind_res < 0) {
		fprintf(stderr, "[ERROR] Failed to bind socket...: %s\n", gai_strerror(bind_res));
		exit(1);
	}

	char *hostname = (char*)calloc(BUFF_SIZE, sizeof(char));
	gethostname(hostname, BUFF_SIZE);
	
	printf("[LOG] The name of the machine hosting the server is: %s\n", hostname);

	freeaddrinfo(res);
	
	return 0;
}
