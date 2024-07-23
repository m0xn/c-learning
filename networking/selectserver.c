#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT "3000"
#define BACKLOG 10
#define MAX_BUFF_SIZE 256
#define INIT_SOCK_ARR_SIZE 5

void* get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*) sa)->sin_addr);
	return &(((struct sockaddr_in6*) sa)->sin6_addr);
}

int setup_server() {
	int sockfd;
	struct addrinfo hints, *p, *servinfo;

	int rv;
	int yes = 1;

	char ip[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	rv = getaddrinfo(NULL, PORT, &hints, &servinfo);
	if (rv == -1) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return -1;
	}

	for (p = servinfo; p != NULL; p = p->ai_next) {
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sockfd == -1) {
			close(sockfd);
			perror("[ERROR] socket");
			continue;
		}

		memset(ip, 0, sizeof ip);
		inet_ntop(p->ai_family, get_in_addr(p->ai_addr), ip, sizeof ip);
		printf("[LOG] Trying to bind the server on %s...\n", ip);

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
			close(sockfd);
			perror("[ERROR] setsockopt");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("[ERROR] bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "[ERROR] The server could not be initialized\n");
		return -1;
	}

	freeaddrinfo(servinfo);

	if (listen(sockfd, BACKLOG) == -1) {
		perror("[ERROR] listen");
		return -1;
	}

	printf("[LOG] Server listening on %s!\n", ip);

	return sockfd; 
}

int main()
{
	int servfd, connfd;
	struct sockaddr_storage conn_addr; 
	socklen_t sin_size = sizeof conn_addr;

	fd_set mainset, readfds;
	size_t nfds;

	char ip[INET6_ADDRSTRLEN], buffer[MAX_BUFF_SIZE];

	int nbytes;

	servfd = setup_server();
	if (servfd == -1)
		exit(1);
	
	FD_ZERO(&mainset);
	FD_SET(servfd, &mainset);

	nfds = servfd;

	printf("Waiting for connections...\n");

	for (;;) {
		readfds = mainset;
		if (select(nfds+1, &readfds, NULL, NULL, NULL) == -1) {
			perror("[ERROR] select");
			continue;
		}

		for (size_t i = 0; i <= nfds; ++i) {
			if (FD_ISSET(i, &readfds)) {
				if (i == (size_t) servfd) {
					// The server is ready to accept a new connection
					connfd = accept(servfd, (struct sockaddr*) &conn_addr, &sin_size);
					if (connfd == -1) {
						perror("[ERROR] accept");
						continue;
					}

					nfds = connfd > (int) nfds ? (size_t) connfd : nfds;
					FD_SET(connfd, &mainset);

					memset(ip, 0, sizeof ip);
					inet_ntop(conn_addr.ss_family, get_in_addr((struct sockaddr*) &conn_addr), ip, sizeof ip);
					printf("Got a connection from %s on socket %d\n", ip, connfd);
				} else {
					// Some client is ready to send a message
					memset(buffer, 0, sizeof buffer);
					nbytes = recv(i, buffer, sizeof buffer, 0);
					if (nbytes == -1) {;
						perror("[ERROR] recv");
						continue;
					} else if (nbytes == 0) {
						printf("[LOG] Connection hung up on socket %d\n", (int) i);
						close(i);
						FD_CLR(i, &mainset);
					}

					// Send the message to the rest of the clients 
					for (size_t j = servfd; j <= nfds; ++j) {
						if (j != (size_t) servfd && j != i && FD_ISSET(j, &mainset)) {
							if (send(j, buffer, sizeof buffer, 0) == -1) {
								perror("[ERROR] send");
								continue;
							}
						}
					}
				}
			}
		}
	}

	close(servfd);
	return 0;
}
