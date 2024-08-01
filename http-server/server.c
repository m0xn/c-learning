#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdint.h>

#define PORT "8080"
#define MAX_BUFF_SIZE 256
#define MAX_FILE_SIZE 256000

typedef enum {
	HTML,
	PNG,
	JPEG,
	ICO,
	WEBP,
	CSS,
	JS
} MIME_types;

// Global MIME types array
char *mime_types[] = {
	"text/html",
	"image/png",
	"image/jpeg",
	"image/vnf.microsoft.icon",
	"image/webp",
	"text/css",
	"text/javascript"
};

char* get_mime_type(char* path)
{
	if (!strstr(path, ".")) {
		fprintf(stderr, "Invalid resource\n");
		return "text/plain";
	}

	char *path_copy = (char*)calloc(strlen(path), sizeof(char));
	char *mime = (char*)calloc(128, sizeof(char));

	strcpy(path_copy, path);

	char *extension = strtok(path_copy, ".");
	extension = strtok(NULL, ".");

	if (strcmp(extension, "html") == 0) {
		mime = mime_types[HTML];
	} else if (strcmp(extension, "png") == 0) {
		mime = mime_types[PNG];
	} else if (strcmp(extension, "jpeg") == 0 || strcmp(extension, "jpg") == 0) {
		mime = mime_types[JPEG];
	} else if (strcmp(extension, "ico") == 0) {
		mime = mime_types[ICO];
	} else if (strcmp(extension, "webp") == 0) {
		mime = mime_types[WEBP];
	} else if (strcmp(extension, "css") == 0) {
		mime = mime_types[CSS];
	} else if (strcmp(extension, "js") == 0) {
		mime = mime_types[JS];
	} else {
		fprintf(stderr, "Unknown MIME type: %s\n", extension);
		exit(1);
	}

	free(path_copy);
	return mime;
}

void* get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*) sa)->sin_addr);
	return &(((struct sockaddr_in6*) sa)->sin6_addr);
}

int setup_server()
{
	int sockfd;
	struct addrinfo hints, *p, *servinfo;
	int rv;
	int yes = 1;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; 
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	rv = getaddrinfo(NULL, PORT, &hints, &servinfo);
	if (rv == -1) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return -1;
	}

	for (p = servinfo; p != NULL; p = p->ai_next) {
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (sockfd == -1) {
			perror("socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) {
			perror("setsockopt");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			perror("bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		close(sockfd);
		fprintf(stderr, "The server socket could not be bound...\n");
		return -1;
	}

	freeaddrinfo(servinfo);

	if (listen(sockfd, 1) == -1) { // For now, just handle one connection at the time
		perror("listen");
		return -1;
	}

	return sockfd;
}

int sendall(int sockfd, char *data, size_t data_size)
{
	size_t remaining_bytes = data_size;
	size_t sent_bytes = 0;

	while (sent_bytes < strlen(data)) {
		if ((sent_bytes += send(sockfd, data + sent_bytes, remaining_bytes, 0)) == (size_t) -1) {
			perror("send");
			return -1;
		}
		remaining_bytes -= sent_bytes;
	}

	return 0;
}

size_t read_file(FILE *stream, char *dst, size_t max_line_length)
{
	size_t content_size = 0;

	char *prev_line, *current_line;
	prev_line = (char*)calloc(max_line_length, sizeof(char));
	current_line = (char*)calloc(max_line_length, sizeof(char));

	content_size += getline(&current_line, &max_line_length, stream);
	strcat(dst, current_line);
	while (strcmp(current_line, prev_line) != 0) {
		strcpy(prev_line, current_line);
		content_size += getline(&current_line, &max_line_length, stream);
		strcat(dst, current_line);
	}

	free(prev_line);
	free(current_line);

	return content_size;
}

size_t read_bin_file(FILE *stream, char *dst)
{
	size_t byte_count = 0;
	unsigned char byte_buff;

	while (fread(&byte_buff, sizeof byte_buff, 1, stream) != 0)
		byte_count++;

	fread(dst, sizeof(char), byte_count, stream);

	return byte_count;
}

void get_resource_path(char *src, char *path_dst)
{
	char *start_line = (char*)calloc(128, sizeof(char));
	strncpy(start_line, src, strcspn(src, "\r\n"));

	// Iterate two times to get the resource being requested
	// GET / HTTP/1.1
	//
	// strtok(start_line, " ") => GET
	// strtok(NULL, " ") (subsequent call) => /

	strtok(start_line, " ");
	strcpy(path_dst, strtok(NULL, " "));
	free(start_line);
}

char* build_res(int code, char* msg, char* mime, char *body, size_t body_size)
{
	char *res = (char*)calloc(MAX_FILE_SIZE+1024, sizeof(char));
	char *tmp = (char*)calloc(128, sizeof(char));

	sprintf(res, "HTTP/1.1 %d %s", code, msg);
	
	sprintf(tmp, "Content-Type: %s\r\n", mime);
	strcat(res, tmp);

	memset(tmp, 0, 128);

	sprintf(tmp, "Content-Length: %ld\r\n", body_size);
	strcat(res, tmp);

	strcat(res, "\r\n");
	strcat(res, body);

	return res;
}

void handle_get(char *req, int sockfd) {
	char *res_path = (char*)calloc(32, sizeof(char));
	get_resource_path(req, res_path);

	if (strcmp(res_path, "/") == 0) {
		strcpy(res_path, "/index.html"); // Default to index.html if no path was specified
	}

	res_path++; // Skip the / at the start of the path

	char *mime = get_mime_type(res_path);

	char *file = (char*)calloc(MAX_FILE_SIZE, sizeof(char));
	size_t file_size;
	int code;
	char *msg;
	char *mode = strstr(mime, "image") ? "rb" : "r";
	
	FILE *resource = fopen(res_path, mode);
	if (resource == NULL) {
		resource = fopen("404.html", "r");
		code = 404;
		msg = "Not Found";
	} else {
		code = 200;
		msg = "OK";
	}

	file_size = strcmp(mode, "r") == 0 ? read_file(resource, file, MAX_BUFF_SIZE) : read_bin_file(resource, file);
	char *res = build_res(code, msg, mime, file, file_size);

	if (sendall(sockfd, res, strlen(res)) == -1) {
		perror("sendall");
		return;
	}
}

int main()
{
	int servfd, connfd;

	struct sockaddr_storage conn_addr;
	socklen_t sin_size = sizeof conn_addr;

	servfd = setup_server();
	if (servfd == -1) {
		fprintf(stderr, "The server could not be initialized...\n");
		exit(1);
	}

	printf("Listening for connections to serve the content...\n");

	for (;;) {
		connfd = accept(servfd, (struct sockaddr*) &conn_addr, &sin_size);
		if (connfd == -1) {
			fprintf(stderr, "Failed to handle connection...");
			exit(1);
		}

		char *req_buff = (char*)calloc(1024, sizeof(char));
		while (recv(connfd, NULL, 1024, MSG_PEEK) != 0) { // Peek at the incoming data;
			recv(connfd, req_buff, 1024, 0); // Read the data afterwards
			handle_get(req_buff, connfd);
			memset(req_buff, 0, 1024);
		}
	}

	close(servfd);
	return 0;
}
