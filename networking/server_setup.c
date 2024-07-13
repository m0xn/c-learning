#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

int main()
{
	int status;
	struct addrinfo hints;
	struct addrinfo *servinfo;

	memset(&hints, 0, sizeof hints); // Makes sure hints is completly in blank
	hints.ai_family = AF_UNSPEC;	 // IP Agnostic server, don't care about IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM; // TCP stream
	hints.ai_flags = AI_PASSIVE;	 // Fill in the IP automatically with localhost

	if ((status = getaddrinfo(NULL, "8080", &hints, &servinfo)) != 0) {
		fprintf(stderr, "Couldn't get address info: %s\n", gai_strerror(status));
		exit(1);
	}

	// Do whatever with the server
	// servinfo should point to a linked list with more addrinfo structures 

	freeaddrinfo(servinfo); // Free the addrinfo linked list
}
