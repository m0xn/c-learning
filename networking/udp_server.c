#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>

#define LISTENING_PORT "3000"
#define BUFF_SIZE 1024
#define BACKGLOG 5

char* addr_port(struct sockaddr *conn_info) {
	char ip[INET_ADDRSTRLEN], port[6];
	char *addr_port = (char*)calloc(sizeof ip + sizeof port, sizeof(char));

	switch (conn_info->sa_family) {
		case AF_INET: {
			struct sockaddr_in *ipv4 = (struct sockaddr_in*) conn_info;
			inet_ntop(ipv4->sin_family, &ipv4->sin_addr, ip, sizeof ip);
			sprintf(port, "%d", ipv4->sin_port);
			break;
		}
		case AF_INET6: {
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6*) conn_info;
			inet_ntop(ipv6->sin6_family, &ipv6->sin6_addr, ip, sizeof ip);
			sprintf(port, "%d", ipv6->sin6_port);
			break;
		}
	}

	memset(addr_port, 0, sizeof ip + sizeof port);

	if (conn_info->sa_family == AF_INET6) {
		strcat(addr_port, "[");
		strcat(addr_port, ip);
		strcat(addr_port, "]");
	} else
		strcat(addr_port ,ip);

	strcat(addr_port, ":");
	strcat(addr_port, port);
	return addr_port;
}

int main()
{
	struct addrinfo hints, *res;
	struct sockaddr sender_addr;
	socklen_t sender_addrlen = sizeof sender_addr;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, LISTENING_PORT, &hints, &res);
	if (errno < 0) {
		fprintf(stderr, "[ERROR] Couldn't get address info due to: %s\n", gai_strerror(errno));
		exit(1);
	}

	int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (errno < 0) {
		perror("[ERROR] Failed to create socket...");
		exit(1);
	}
	
	bind(socket_fd, res->ai_addr, res->ai_addrlen);
	
	int yes = 1;
	setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

	// For this kind of server we don't need to listen anymore for connections
	// because the data transmission is going connectionless.
	
	char *buffer = (char*)calloc(BUFF_SIZE, sizeof(char));
	recvfrom(socket_fd, buffer, BUFF_SIZE, 0, &sender_addr, &sender_addrlen);

	printf("Message recieved from %s -> \"%s\"\n", addr_port(&sender_addr), buffer);

	const char *ack_message = "Recieved client message!";
	size_t sent_bytes = sendto(socket_fd, ack_message, strlen(ack_message), 0, (struct sockaddr*)  &sender_addr, sender_addrlen);
	
	sent_bytes == strlen(ack_message)
		? printf("[LOG] Sent (%ld bytes)\n", strlen(ack_message))
		: printf("[WARN] The full message couldn't be sent to the server (%ld/%ld bytes sent)\n", sent_bytes, strlen(ack_message));

	freeaddrinfo(res);

	return 0;
}
