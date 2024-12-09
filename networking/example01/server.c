// Example of local namespace socket

#include<stdio.h>
#include<stddef.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/un.h>

int make_named_socket(const char *filename) {
	int sock = socket(PF_LOCAL, SOCK_DGRAM, 0);
	if(sock < 0) {
		perror("socket");
		return 1;
	}

	struct sockaddr_un name;
	name.sun_family = AF_LOCAL;
	strncpy(name.sun_path, filename, sizeof(name.sun_path));
	name.sun_path[sizeof(name.sun_path) - 1] = '\0';

	size_t size;
	size = SUN_LEN(&name); 
	if(bind(sock, (struct sockaddr*)&name, size) < 0) {
		perror("bind");
		return 1;
	}

	return sock;
}

int main(int argc, char* argv[]) {
	const char* const socket_name = argv[1];
	int sock = make_named_socket(socket_name);
	printf("sock=%d\n", sock);
	int close_status = shutdown(sock, 2);
	if(close_status < 0) {
		perror("shutdown");
		return 1;
	}
	return 0;
}
