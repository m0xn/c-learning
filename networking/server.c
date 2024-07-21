#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

#define LISTENING_PORT "3000"
#define BACKLOG 5
#define BUFF_SIZE 1024

char* addr_port(struct sockaddr *conn_info) {
	char ip[INET_ADDRSTRLEN], port[6];
	char *addr_port = (char*)calloc(sizeof ip + sizeof port, sizeof(char));

	switch (conn_info->sa_family) {
		case AF_INET: {
			struct sockaddr_in *ipv4 = (struct sockaddr_in*) conn_info;
			inet_ntop(ipv4->sin_family, &ipv4->sin_addr, ip, sizeof ip);
			sprintf(port, "%u", ipv4->sin_port);
			break;
		}
		case AF_INET6: {
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6*) conn_info;
			inet_ntop(ipv6->sin6_family, &ipv6->sin6_addr, ip, sizeof ip);
			sprintf(port, "%u", ipv6->sin6_port);
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
	int status;
	struct addrinfo hints, *res;

	struct sockaddr connection_addr;
	socklen_t addr_len;

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

	printf("[LOG] Server listening on %s\n", addr_port(res->ai_addr));

	int listen_res = listen(socket_fd, BACKLOG);
	if (listen_res < 0) {
		fprintf(stderr, "[ERROR] Failed to start listening on port %s...\n", LISTENING_PORT);
		exit(1);
	}

	int conn_fd = accept(socket_fd, &connection_addr, &addr_len);
	if (conn_fd < 0) {
		fprintf(stderr, "[ERROR] Failed to accept connection...\n");
		exit(1);
	}

	printf("[LOG] Client %s connected to the server\n", addr_port(&connection_addr));

	const char *msg = "Hello, client!";
	send(conn_fd, msg, strlen(msg), 0);

	freeaddrinfo(res);
	
	return 0;
}
