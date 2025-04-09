#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>

#define BUFFER_SIZE 2
#define SENTINEL '\0'

typedef enum TokenType {
	CREATE, 
	TABLE, 
	SELECT, 
	FROM, 
	WHERE, 
	SPACE
}TokenType;

int loadBufferContent(char *buffer, int fd) {
	int n = read(fd, buffer, BUFFER_SIZE);
	buffer[BUFFER_SIZE] = SENTINEL;
	return n;
}

TokenType getToken(char *buffer, char *lexemeBegin, char *forward) {
	// start from lexemeBegin 
	// check the current character 
	// if space then proceed forward
	// if alphabetic then reach the end of the current lexeme, match with any possible token type 

	return SPACE;
}

int main(int argc, char* argv[]) {
	if(argc < 2) {
		fprintf(stdout, "format: %s filename\n", argv[0]);
		return 1;
	}

	// 2 buffer scheme
	char buffers[2][BUFFER_SIZE + 1];
	int fd = open(argv[1], O_RDONLY, S_IRUSR);
	int activeBuf = 0, n = 0;

	char *lexemeBegin, *forward;
	TokenType token;
	while((n = loadBufferContent(buffers[activeBuf], fd)) > 0) {
		// activeBuf buffer has the source code (partial)
		
		// lexemeBegin will point to the start of current lexeme and forward will proceed untill a valid token is found
		lexemeBegin = buffers[activeBuf];
		forward = lexemeBegin;

		token = getToken(buffers[activeBuf], lexemeBegin, forward);
		switch(token) {
			case CREATE:
				printf("<CREATE>\n");
				break;
			default:
				printf("<TOKEN\n");
		}

		activeBuf = 1 - activeBuf;
	}

	close(fd);
	return 0;
}
