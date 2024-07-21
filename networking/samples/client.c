// client.c -> Simple stream TCP client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define PORT "3000"
#define MAX_BUFF_SIZE 100

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)sa)->sin_addr);
    return &(((struct sockaddr_in6*)sa)->sin6_addr);}

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: ./client hostname\n");
		exit(1);
	}
	
	int sockfd;
	struct addrinfo hints, *p, *servinfo;
	char buffer[MAX_BUFF_SIZE], ip[INET6_ADDRSTRLEN];
	int rv, num_bytes;
	
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	rv = getaddrinfo(argv[1], PORT, &hints, &servinfo);
	if (rv == -1) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// Loop to try with all address infos provided by getaddrinfo()
	for(p = servinfo; p != NULL; p = p->ai_next) {
		inet_ntop(p->ai_family, get_in_addr(p->ai_addr), ip, sizeof ip);
		printf("client: Trying to connect to %s...\n", ip);

		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sockfd == -1) {
			close(sockfd);
			perror("socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("connect");
			continue;
		}
		printf("client: Connected to %s!\n", ip);

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "client: Failed to connect to the server\n");
		exit(1);
	}

	freeaddrinfo(servinfo);

	// Recieve the data from the server
	if ((num_bytes = recv(sockfd, buffer, sizeof buffer, 0)) == -1) {
		perror("recv");
		exit(1);
	}

	buffer[num_bytes] = '\0'; // Add null-terminator at the end of the recieved message

	printf("client: Message recieved from the server -> \"%s\"\n", buffer);

	char *buff = (char*)calloc(MAX_BUFF_SIZE, sizeof(char));

	printf("Introduce your message for the chat:\n> ");
	scanf("%[^\n]", buff);

	if ((num_bytes = send(sockfd, buff, sizeof buff, 0)) == -1) {
		perror("send");
		exit(1);
	}

	close(sockfd);

	return 0;
}
