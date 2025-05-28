#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

#define BUFFER_SIZE 1

int main(int argc, char* argv[]) {
	if(argc < 2) {
		fprintf(stdout, "Format: %s filename\n", argv[0]);
		return 1;
	}

	int fd = open(argv[1], O_RDONLY, S_IRUSR);
	
	char buffer[BUFFER_SIZE+1];
	int n = 0;
	while((n = read(fd, buffer, BUFFER_SIZE)) > 0) {
		buffer[n] = '\0';

		if(write(STDOUT_FILENO, buffer, n) == -1) {
			perror("write");
			return 1;
		}
	}

	close(fd);
	return 0;
}
