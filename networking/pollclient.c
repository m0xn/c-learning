#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT "3000"
#define MAX_BUFF_SIZE 256

void* get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*) sa)->sin_addr);
	return &(((struct sockaddr_in6*) sa)->sin6_addr);
}

int main(int argc, char **argv) {
	if (argc != 3) {
		printf("Usage: ./client HOST USERNAME");
		return 1;
	}

	int sockfd;
	struct addrinfo hints, *p, *servinfo;

	int rv;
	int yes = 1;

	char ip[INET6_ADDRSTRLEN];
	size_t max_buff_size = MAX_BUFF_SIZE;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	rv = getaddrinfo(argv[1], PORT, &hints, &servinfo);
	if (rv == -1) {
		fprintf(stderr, "[ERROR] getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	for (p = servinfo; p != NULL; p = p->ai_next) {
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sockfd == -1) {
			close(sockfd);
			perror("[ERROR] client -> socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
			close(sockfd);
			perror("[ERROR] client -> setsockopt");
			continue;
		}

		if (fcntl(sockfd, F_SETFD, O_NONBLOCK) == -1) {
			close(sockfd);
			perror("[ERROR] client -> fcntl");
			continue;
		}

		inet_ntop(p->ai_family, get_in_addr(p->ai_addr), ip, sizeof ip);
		printf("Trying to connect to %s...\n", ip);

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("[ERROR] client -> connect");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "[ERROR] Client could not connect to the server\n");
		return 1;
	}

	// Send the username to the server after connection
	if (send(sockfd, argv[2], strlen(argv[2]), 0) == -1) {
		perror("[ERROR] client -> send");
		return 1;
	}

	for (;;) {
		char *send_buff = (char*)calloc(max_buff_size, 1);
		char *recv_buff = (char*)calloc(max_buff_size, 1);

		printf("%s> ", argv[2]);
		getline(&send_buff, &max_buff_size, stdin);

		if (recv(sockfd, recv_buff, max_buff_size, 0) == -1)
			continue;
		printf("%s\n", recv_buff);

		if (send(sockfd, send_buff, strlen(send_buff), 0) == -1) {
			perror("[ERROR] client -> send");
			continue;
		}
	}
}
