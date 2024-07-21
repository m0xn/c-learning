// talker.c -> a datagram "client" demo
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

int main(int argc, char **argv)
{
	if (argc != 3) {
		printf("Usage: ./talker HOSTNAME MESSAGE");
		exit(1);
	}

	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int num_bytes;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET6; // IPv6 to talk with listener.c 
	hints.ai_socktype = SOCK_DGRAM; // Setup for UDP
	
	rv = getaddrinfo(argv[1], PORT, &hints, &servinfo);
	if (rv == -1) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// Loop through all addrs to make a socket
	for (p = servinfo; p != NULL; p = p->ai_next) {
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sockfd == -1) {
			close(sockfd);
			perror("talker -> socket");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "talker: Failed to create socket\n");
		exit(1);
	}

	num_bytes = sendto(sockfd, argv[2], strlen(argv[2]), 0, p->ai_addr, p->ai_addrlen);
	if (num_bytes == -1) {
		perror("sendto");
		exit(1);
	}

	freeaddrinfo(servinfo);

	printf("talker: sent %d bytes to %s\n", num_bytes, argv[1]);
	close(sockfd);

	return 0;
}
