// listener.c -> a DGRAM sockets "server" 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT "3000"
#define MAX_BUFF_SIZE 100

void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
        return &(((struct sockaddr_in*)sa)->sin_addr);
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// Explanation of hints.ai_family = AF_INET6.
// Since in this case we're going to transmit packets of data connectionless
// We cannot ensure that both the machine and the client will send those
// packets using the same IP version standard. That would result in an error
// So for that matter, we're ensuring that the packets are being transmitted
// via IPv6 to avoid errors when receiving data.

int main()
{
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int num_bytes;
	struct sockaddr_storage sender_addr;
	char buffer[MAX_BUFF_SIZE];
	socklen_t addr_len = sizeof sender_addr;
	char ip[INET6_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET6; // Setup for IPv6
	hints.ai_socktype = SOCK_DGRAM; // Setup for UDP
	hints.ai_flags = AI_PASSIVE; // use local machine's IP

	rv = getaddrinfo(NULL, PORT, &hints, &servinfo);
	if (rv == -1) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the addrs from getaddrinfo() and bind to the first possible one
	for (p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("socket");
			continue;
		};

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "listener: Failed to bind socket\n");
		exit(1);
	}

	freeaddrinfo(servinfo);

	printf("listener: waiting from recvfrom()...\n");

	if ((num_bytes = recvfrom(sockfd, buffer, MAX_BUFF_SIZE-1, 0, (struct sockaddr*) &sender_addr, &addr_len)) == -1) { // Account for the null-terminator at the end of the received mesasge
		perror("revfrom");
		exit(1);
	}

	inet_ntop(sender_addr.ss_family, get_in_addr((struct sockaddr*) &sender_addr) ,ip, sizeof ip);
	printf("listener: got a packet from %s\n", ip);
	printf("listener: packet is %d bytes long\n", num_bytes);
	buffer[num_bytes] = '\0';
	printf("listener: packet contains \"%s\"\n", buffer);

	close(sockfd);

	return 0;
}
