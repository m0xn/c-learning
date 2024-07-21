// polserver.c -> my implementation for a simple multiparty chat server 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <poll.h>
#include <unistd.h>
#include <errno.h>

#define PORT "3000"
#define BACKLOG 10
#define INIT_POLL_SIZE 5
#define INIT_CONN_SIZE 5
#define BUFF_MAX_SIZE 256
#define MAX_USRN_LEN 32

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)sa)->sin_addr);
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void add_fd(struct pollfd **pfds, size_t *pfds_count, size_t *pfds_size, int filde)
{
	if (*pfds_count == *pfds_size) {
		*pfds_size *= 2;
		*pfds = (struct pollfd*)reallocarray(*pfds, *pfds_size, sizeof(**pfds));
	}

	struct pollfd *fd = (struct pollfd*)malloc(sizeof(**pfds));
	fd->fd = filde;
	fd->events = POLLIN;
	(*pfds)[(*pfds_count)++] = *fd;
}

void rm_fd(struct pollfd *pfds, size_t *pfds_count, int i) {
	if (*pfds_count == 0) {
		fprintf(stderr, "The poll file descriptor array is empty\n");
		return;
	}

	close(pfds[i].f);
	pfds[i] = pfds[--(*pfds_count)];
}

int main()
{
	int sockfd, newfd;
	size_t nfds = 0;
	size_t pfds_size = INIT_POLL_SIZE; 
	struct pollfd *pfds = (struct pollfd*)calloc(pfds_size, sizeof(*pfds));

	struct addrinfo hints, *p, *servinfo;
	struct sockaddr_storage sender_addr;
	socklen_t sin_size = sizeof sender_addr;

	int yes = 1;
	char ip[INET6_ADDRSTRLEN], buff[BUFF_MAX_SIZE]; // Sender's ip address
	int rv; // Response value from the getaddrinfo() call
	int num_events;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // Set the local machine's ip

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo))) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// Loop through all the results and bind to the first we can bind to
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			close(sockfd);
			perror("server -> socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server -> bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo);

	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("server -> listen");
		exit(1);
	}

	// Add the listener socket to the poll
	pfds[0].fd = sockfd;
	pfds[0].events = POLLIN;

	nfds = 1;

	printf("server: waiting for connections...\n");

	for(;;) {
		num_events = poll(pfds, nfds, -1); // Wait forever
		int handled_events = 0;

		if (num_events == -1) {
			perror("server -> poll");
			continue;
		}

		for (size_t i = 0; i < nfds; ++i) {
			if (pfds[i].revents & POLLIN && handled_events < num_events) {
				handled_events++;
				if (pfds[i].fd == sockfd) {
					// The server is ready to accept a connection
					newfd = accept(sockfd, (struct sockaddr*) &sender_addr, &sin_size);
					if (newfd == -1) {
						perror("server -> accept");
						continue;
					}

					add_fd(&pfds, &nfds, &pfds_size, newfd);

					inet_ntop(sender_addr.ss_family, get_in_addr((struct sockaddr*) &sender_addr), ip, sizeof ip);
					printf("Connection from %s added to poll on socket %d\n", ip, newfd);
				} else {
					// This is a normal client which sent info
					memset(buff, 0, sizeof buff);

					int res = recv(pfds[i].fd, buff, sizeof buff, 0);
					if (res == -1) {
						perror("server -> recv");
						continue;
					} else if (res == 0) {
						printf("Socket %d hung up the connection\n", pfds[i].fd);
						rm_fd(pfds, &nfds, i);
						continue;
					}

					// Send the message to all the rest of the clients
					for (size_t j = 0; j < nfds; ++j) {
						if (pfds[j].fd != sockfd && pfds[j].fd != pfds[i].fd) {
							if (send(pfds[j].fd, buff, sizeof buff, 0) == -1) {
								perror("server -> send");
								continue;
							}
						}
					}
				}
			}
		}
	}

	return 0;
}
