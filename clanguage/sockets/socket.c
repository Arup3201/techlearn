#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>

int main(int argc, char* argv[]) {

	if(argc < 3) {
		fprintf(stderr, "%s: needs host and port\n", argv[0]);
		return 1;
	}

	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd < 0) {
		fprintf(stderr, "Error: failed to create socket - %s\n", strerror(errno));
		return 1;
	}

	struct sockaddr_in sock_addr;
	sock_addr.sin_family = PF_INET;
	if(inet_pton(AF_INET, argv[1], &sock_addr.sin_addr) <= 0) {
		fprintf(stderr, "Error: failed to convert address from text to internet format - %s\n", strerror(errno));
		return 1;
	}
	sock_addr.sin_port = htons(atoi(argv[2]));

	if(bind(sock_fd, (struct sockaddr*)&sock_addr, sizeof(sock_addr)) < 0) {
		fprintf(stderr, "Error: failed to assign address to socket - %s\n", strerror(errno));
		return 1;
	}

	if(listen(sock_fd, 1) < 0) {
		fprintf(stderr, "Error: failed to listen to client requests - %s\n", strerror(errno));
		return 1;
	}

	struct sockaddr_in client_addr;
	int addr_len = sizeof(client_addr);
	int client_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &addr_len);
	if(client_fd < 0) {
		fprintf(stderr, "Error: failed to accept client connection - %s\n", strerror(errno));
		return 1;
	}
	
	ssize_t address_string_len = 32;
	char client_address_string[address_string_len];
	inet_ntop(AF_INET, &client_addr.sin_addr, client_address_string, address_string_len);
	fprintf(stdout, "INFO: Connection established to client at address %s\n", client_address_string);


	char *http_prefix = "HTTP/1.1 200 \r\nContent-Type: text/html\r\nContent-Length: %d\r\n\r\n";
	char *message = "Hi Client!!";

	char *response = malloc(1024);
	sprintf(response, http_prefix, strlen(message));
	response = strcat(response, message);
	send(client_fd, response, strlen(response), 0);

	shutdown(client_fd, SHUT_RDWR);
	return 0;
}
