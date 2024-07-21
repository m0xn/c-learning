#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>

#define PORT "3000"
#define MAX_BUFF_SIZE 256
#define BACKLOG 10
#define MAX_USRN_LEN 32
#define INIT_USRS_SIZE 5

struct usr {
	int ufd;
	char username[MAX_USRN_LEN];
};

void* get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*) sa)->sin_addr);
	return &(((struct sockaddr_in6*) sa)->sin6_addr);
}

void strip_usrn(char *usrn) {
	char *temp = (char*)calloc(strlen(usrn), sizeof(char));
	strcpy(temp, usrn);
	const size_t usrn_len = strcspn(usrn, "/r/n");
	strncpy(usrn, temp, usrn_len);
	free(temp);
}

void add_usr(struct pollfd **pfds, struct usr **usrs, size_t *usrs_count, size_t *usrs_size, char *usrn, int filde)
{
	if (*usrs_count == *usrs_size) {
		*usrs_size *= 2;
		*pfds = (struct pollfd*)reallocarray(*pfds, *usrs_size, sizeof **pfds);
		*usrs = (struct usr*)reallocarray(*usrs, *usrs_size, sizeof **usrs);
	}

	(*pfds)[*usrs_count].fd = filde;
	(*pfds)[*usrs_count].events = POLLIN;

	(*usrs)[*usrs_count].ufd = filde;
	memset((*usrs)[*usrs_count].username, 0, MAX_USRN_LEN);
	strcpy((*usrs)[*usrs_count].username, usrn);

	(*usrs_count)++;
}

void rm_usr(struct pollfd *pfds, struct usr *usrs, size_t *usrs_count, size_t i)
{
	if (!usrs_count) {
		fprintf(stderr, "[ERROR]: There are no users connected to the server");
		return;
	}

	close(pfds[i].fd);
	pfds[i] = pfds[(*usrs_count)--];
	usrs[i] = usrs[(*usrs_count)--];
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
		fprintf(stderr, "[ERROR]: getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	for (p = servinfo; p != NULL; p = p->ai_next) {
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sockfd == -1) {
			close(sockfd);
			perror("[ERROR] socket");
			continue;
		}

		inet_ntop(p->ai_family, get_in_addr(p->ai_addr), ip, sizeof ip);
		printf("[LOG] Trying to bind on %s...\n", ip);

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

	printf("[LOG] Server bind at %s:%s!\n", ip, PORT);

	if (p == NULL) {
		fprintf(stderr, "[ERROR] Couldn't bind server...\n");
		return -1;
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("[ERROR] listen");
		return -1;
	}

	freeaddrinfo(servinfo);

	return sockfd;
}

int main()
{
	int servfd, connfd;
	struct sockaddr_storage conn_addr;
	socklen_t sin_size = sizeof conn_addr;

	size_t usrs_count = 0, usrs_size = INIT_USRS_SIZE;
	struct pollfd *pfds = (struct pollfd*)calloc(usrs_size, sizeof *pfds);
	struct usr *usrs = (struct usr*)calloc(usrs_size, sizeof *usrs);

	char ip[INET6_ADDRSTRLEN];
	char buffer[MAX_BUFF_SIZE], usrn_buffer[MAX_USRN_LEN];

	int nbytes;
	size_t num_events;

	servfd = setup_server();
	if (servfd == -1) {
		fprintf(stderr, "[ERROR] Couldn't start server...\n");
		return 1;
	}

	pfds[0].fd = servfd;
	pfds[0].events = POLLIN;

	usrs[0].ufd = servfd;
	strcpy(usrs[0].username, "server");

	usrs_count = 1;

	printf("[LOG] Waiting for connections...\n");

	for (;;) {
		num_events = poll(pfds, usrs_count, -1);

		if (num_events == (size_t) -1) {
			perror("[ERROR] poll");
			continue;
		}

		for (size_t i = 0; i < usrs_count; ++i) {
			if (pfds[i].revents & POLLIN) {
				if (pfds[i].fd == servfd) {
					// Server ready to accept a connection from the client
					connfd = accept(servfd, (struct sockaddr*) &conn_addr, &sin_size);
					if (connfd == -1) {
						perror("[ERROR] connfd");
						continue;
					}
					
					// Recieve the username the user selected to connect to the server
					memset(usrn_buffer, 0, sizeof usrn_buffer);
					nbytes = recv(connfd, usrn_buffer, sizeof usrn_buffer, 0);
					if (nbytes == -1) {
						perror("[ERROR] recv");
						continue;
					} else if (nbytes == 0) {
						printf("[LOG] Client on socket %d disconnected\n", connfd);
						rm_usr(pfds, usrs, &usrs_count, i);
						continue;
					}

					add_usr(&pfds, &usrs, &usrs_count, &usrs_size, usrn_buffer, connfd);;

					inet_ntop(conn_addr.ss_family, get_in_addr((struct sockaddr*) &conn_addr), ip, sizeof ip);
					strip_usrn(usrn_buffer);

					printf("Got a connection from %s on socket %d with the username: %s\n", ip, connfd, usrn_buffer);
				} else {
					// Connected client wanting to send in a message
					memset(buffer, 0, sizeof buffer);
					if (recv(pfds[i].fd, buffer, sizeof buffer, 0) == -1) {
						perror("[ERROR] recv");
						continue;
					}

					strip_usrn(usrs[i].username);

					char formatted_buffer[MAX_USRN_LEN+MAX_BUFF_SIZE+2];
					memset(formatted_buffer, 0, sizeof formatted_buffer);
					sprintf(formatted_buffer, "%s> ", usrs[i].username);
					strcat(formatted_buffer, buffer);
					
					// Send to the rest of the users connected to the server
					for (size_t j = 0; j < usrs_count; ++j) {
						if (pfds[j].fd != servfd && pfds[j].fd != pfds[i].fd) {
							if (send(pfds[j].fd, formatted_buffer, sizeof formatted_buffer, 0) == -1) {
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
