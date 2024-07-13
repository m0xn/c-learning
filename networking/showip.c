#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>

// showip.c show the ip address of the host specified in the console

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: showip [HOST]\n");
		exit(1);
	}

	int status;
	char ipstr[INET6_ADDRSTRLEN];

	struct addrinfo hints, *res, *p;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
		fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(status));
		exit(1);
	}

	printf("IP addresses for \"%s\":\n\n", argv[1]);

	for (p = res; p != NULL; p = p->ai_next) {
		void *addr;
		char *ipver;

		if (p->ai_family == AF_INET) {
			struct sockaddr_in *ipv4 = (struct sockaddr_in*) p->ai_addr;
			addr = &ipv4->sin_addr;
			ipver = "IPv4";
		} else {
			struct sockaddr_in6 *ipv6 = (struct sockaddr_in6*) p->ai_addr;
			addr = &ipv6->sin6_addr;
			ipver = "IPv6";
		}

		inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
		printf("%s -> %s\n", ipver, ipstr);
	}

	freeaddrinfo(res);
	return 0;
}
