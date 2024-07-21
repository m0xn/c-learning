// server.c -> Simple stream TCP server
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT "3000"
#define BACKLOG 10

void sigchld_handler()
{
	// waitpd() might overwrite errno, so we save a copy beforehand
	int saved_errno = errno;

	while (waitpid(-1, NULL, WNOHANG) > 0);

	errno = saved_errno;
}

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)sa)->sin_addr);
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main()
{
	int sockfd, new_fd;
	struct addrinfo hints, *p, *servinfo;
	struct sockaddr_storage sender_addr;
	socklen_t sin_size;
	struct sigaction sa;
	int yes = 1;
	char ip[INET6_ADDRSTRLEN]; // Sender's ip address
	int rv; // Response value from the getaddrinfo() call

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
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo);

	if (p == NULL) {
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	freeaddrinfo(p);

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // Get all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;

	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	printf("server: waiting for connections...\n");

	while(1) {
		sin_size = sizeof sender_addr;
		new_fd = accept(sockfd, (struct sockaddr*) &sender_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

		inet_ntop(sender_addr.ss_family, (struct sockaddr*) &sender_addr, ip, INET6_ADDRSTRLEN);
		printf("server: got connection from %s\n", ip);

		if (!fork()) { // Equivalent to saying fork() == 0, which means that the child process succesffully created 
			close(sockfd); // child doesn't need a listener
			if (send(new_fd, "Hello, client!", 14, 0) == -1)
				perror("send");
			close(new_fd);
			exit(0);
		}
		close(new_fd); // parent doesn't need this file descriptor
	}

	return 0;
}
