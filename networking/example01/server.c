#include<stdio.h>
#include<stdint.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>

int make_socket(uint16_t port) {
	// Create a socket
	int fd = socket(PF_INET, SOCK_STREAM, 0);
	if(fd < 0) {
		printf("failed to create socket\n");
		return 1;
	}

	// Bind the socket with an address                       
        struct sockaddr_in name;
        name.sin_family = AF_INET;
        name.sin_port = htons(port);
        name.sin_addr.s_addr = htonl(INADDR_ANY);
        if(bind(fd, (struct sockaddr*) &name, sizeof(name)) < 0) {
                printf("failed to bind server address\n");
                return 1;
        }

	return fd;
}

int main() {
	int sock = make_socket(80);
	printf("socket=%d\n", sock);
	return 0;
}
