# I/O in C

Reading from file is a little tricky so I am creating a seperate folder just for that. 

Usually I create a buffer, open a file and read `BUFFER_SIZE` content from it. Then assign the content inside the `buffer`. But this does not ensure that we have read all the things from the file.

To make sure we have read everything we need to run a loop untill the read call returns negative value, which means it has nothing to read anymore.

```c
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
```
