#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>

int main() {
	int fd = socket(PF_INET, SOCK_STREAM, 0);
	if(fd < 0) {
		printf("socket connection failed!\n");
		return 1;
	}
	// Bind the socket with an address
	struct sockaddr_in socket_name;
	uint16_t port = 80; 
	socket_name.sin_family = AF_INET;
	socket_name.sin_port = htons(port);
	socket_name.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(fd, (struct sockaddr*)&socket_name, sizeof(socket_name)) < 0) {
		printf("failed to bind server address\n");
		return 1;
	}

	return 0;
}
