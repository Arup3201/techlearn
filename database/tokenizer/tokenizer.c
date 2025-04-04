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

bool isDigit(char ch) {
	if(ch >= 0x0030 && ch <= 0x0039) {
		return true;
	}

	return false;
}

bool isAlpha(char ch) {
	if((ch >= 0x0041 && ch <= 0x005a) || (ch >= 0x0061 && ch <= 0x007a)) {
		return true;
	}

	return false;
}

bool isSpecial(char ch) {
	return (!isDigit(ch) || !isAlpha(ch) || !isSpace(ch));
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

		switch(*ptr) {
			case isDigit(*ptr): 
				while(isDigit(*ptr) {
					
				}
				break;

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
