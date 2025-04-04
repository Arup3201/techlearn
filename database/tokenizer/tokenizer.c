#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdbool.h>

#define MAX_BUFFER_SIZE 1024

bool isSpace(char ch) {
	return (ch == 0x0009) || (ch == 0x000a) || (ch == 0x000d) || (ch == 0x0020);
}

bool isComment(const char** ptr) {
	// single line comments (--)
	if(*ptr[0] == 0x002d || *ptr[1] == 0x002d) {
		*ptr += 2;
		while(**ptr != '\n') (*ptr)++;
		return true;
	}

	// multi line comments (/* ... */)
	if(**ptr == 0x002f && *(*ptr+1) == 0x002a) {
		(*ptr) += 2;
		while(**ptr != 0x002a && *(*ptr+1) != 0x002f) (*ptr)++;
		(*ptr) += 1;
		return true;
	}

	return false;
}

bool isDigit(char **ptr) {
	// character is in between 0 and 9
	if(**ptr >= 0x0030 && **ptr <= 0x0039) {
		while(**ptr >= 0x0030 && **ptr <= 0x0039) {
			(*ptr)++;
		}
		return true;
	}

	return false;
}


/** 
 * tokenize the sql text input `sql`
 */
void tokenize(const char *ptr) {
	while(*ptr) {
		while(isSpace(*ptr) || isComment(&ptr)) {
			fprintf(stdout, "\n");
			ptr++;
		}
		fprintf(stdout, "%c", *ptr);
		ptr++;
	}
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
