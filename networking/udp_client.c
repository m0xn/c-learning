#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>

#define BUFF_SIZE 1024

int main(int argc, char **argv)
{
	if (argc != 3) {
		printf("Usage: ./udp_client HOST PORT\n");
		exit(1);
	}

	int status;
	struct addrinfo hints, *res;
	struct sockaddr_storage sender_addr;
	socklen_t sender_addrlen = sizeof sender_addr;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET; // IP version agnostic
	hints.ai_socktype = SOCK_DGRAM; // Setup for UDP

	status = getaddrinfo(argv[1], argv[2], &hints, &res);
	if (status < 0) {
		fprintf(stderr, "[ERROR] Couldn't get address info for host: %s in port: %s, due to %s\n", argv[1], argv[2], gai_strerror(status));
		exit(1);
	}

	int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (socket_fd < 0) {
		fprintf(stderr, "[ERROR] Failed initialize socket...\n");
		exit(1);
	}
	
	// Here we don't actually connect to the server because UDP is a connectionless protocol
	// That meaning that there's no previous connection being established between the two
	// parties implied in the communication process. Instead, we will be passing the destination
	// of packets every time we want to send data to the server

	// Sending data connectoinless
	const char *msg = "Hello, server!";
	size_t bytes_sent = sendto(socket_fd, msg, strlen(msg), 0, res->ai_addr, res->ai_addrlen);

	bytes_sent == strlen(msg)
		? printf("[LOG] Sent (%ld bytes)\n", strlen(msg))
		: printf("[WARN] The full message couldn't be sent to the server (%ld/%ld bytes sent)\n", bytes_sent, strlen(msg));

	char *buffer = (char*)calloc(BUFF_SIZE, sizeof(char));
	recvfrom(socket_fd, buffer, BUFF_SIZE, 0, (struct sockaddr*) &sender_addr, &sender_addrlen);
	printf("Confirmation package recieved from the server: %s\n", buffer);

	freeaddrinfo(res);

	return 0;
}
