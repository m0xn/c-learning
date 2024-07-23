// sendall.c -> a function implementation to handle partial send()s
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>

int sendall(int sockfd, char *buffer, int *buff_len) {
	int bytesleft = *buff_len; // Amount of bytes left to send
	int bytessent = 0; // Number of bytes actually sent 

	while (bytessent < *buff_len) {
		bytessent = send(sockfd, buffer+bytessent, bytesleft, 0); // buffer+bytessent offsets the buffer to send out the remaining part of it
		if (bytessent == -1)
			break;
		bytesleft -= bytessent;
	}

	return -(bytessent == -1); // Return -1 on failure 0 on success
}
