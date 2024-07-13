#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

#define BUFF_SIZE 1024
#define DEBUG 0

int main(int argc, char **argv)
{
	if (argc != 3) {
		fprintf(stderr, "Usage: ./client HOST PORT\n");
		exit(1);
	}

	int status;
	struct addrinfo hints, *res;
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	status = getaddrinfo(argv[1], argv[2], &hints, &res);
	if (status < 0) {
		fprintf(stderr, "[ERROR] Failed to get addrinfo: %s\n", gai_strerror(status));
		exit(1);
	}

	int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (socket_fd < 0) {
		fprintf(stderr, "[ERROR] Failed to initliaze socket...\n");
		exit(1);
	}

	int conn_res = connect(socket_fd, res->ai_addr, res->ai_addrlen);
	if (conn_res < 0) {
		fprintf(stderr, "[ERROR] Failed to connect to the server...\n");
		exit(1);
	}

	char *buffer = (char*)calloc(BUFF_SIZE, sizeof(char));
	memset(buffer, 0, BUFF_SIZE);

#ifdef DEBUG
#if DEBUG == 1
	int recieved_bytes = recv(socket_fd, buffer, BUFF_SIZE, 0);
	printf("[LOG] Message from the sever: \"%s\"\n", buffer);
	printf("[DEBUG] Client recieved %d bytes from the server\n", recieved_bytes);
#else
	recv(socket_fd, buffer, BUFF_SIZE, 0);
	printf("[LOG] Message from the sever: \"%s\"\n", buffer);
#endif
#endif
	
	return 0;
}
