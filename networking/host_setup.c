#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>

int main()
{
	int status;
	struct addrinfo hints;
	struct addrinfo *info;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM; 

	if ((status = getaddrinfo("www.google.com", "8080", &hints, &info)) != 0) {
		fprintf(stderr, "Couldn't resolve info: %s\n", gai_strerror(status));
		exit(1);
	}

	// You would have info filled with all the required information 

	struct sockaddr_in *addr_info = (struct sockaddr_in*) info->ai_addr;
	char ip_addr[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &addr_info->sin_addr, ip_addr, INET_ADDRSTRLEN);
	printf("The IP of the targeted node is: %s\n", ip_addr);
	freeaddrinfo(info);
}
