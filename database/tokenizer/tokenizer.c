#include<stdio.h>
#include<locale.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdbool.h>

#define MAX_BUFFER_SIZE 1024

/** 
 * tokenize the sql text input `sql`
 */
void tokenize(const char *sql) {
}

int main(int argc, char *argv[]) {
	if(argc < 2) {
		fprintf(stderr, "Usage: %s filename.c\n", argv[0]);
		return 1;
	}

	char *tFilename = argv[1];
	int fd = open(tFilename, O_RDONLY, S_IRUSR);
	char buffer[MAX_BUFFER_SIZE];
	read(fd, buffer, MAX_BUFFER_SIZE);

	tokenize(buffer);

	return 1;
}
