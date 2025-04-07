#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

#define BUFFER_SIZE 2

int main(int argc, char* argv[]) {
	if(argc < 2) {
		fprintf(stdout, "format: %s filename\n", argv[0]);
		return 1;
	}

	// 2 buffer scheme
	char buffer1[BUFFER_SIZE+1], buffer2[BUFFER_SIZE+1];
	buffer1[BUFFER_SIZE] = EOF;
	buffer2[BUFFER_SIZE] = EOF;

	char *ioBuf = buffer1;

	int fd = open(argv[1], O_RDONLY, S_IRUSR), n=0;
	while((n = read(fd, ioBuf, BUFFER_SIZE)) > 0) {
		if(n >= BUFFER_SIZE) {
			if(ioBuf == buffer1) ioBuf = buffer2;
			else ioBuf = buffer1;
		}

		fprintf(stdout, "%s", ioBuf);
	}
	printf("\n");
	return 0;
}
