#include<stdio.h>
#include<fcntl.h>

void tokenize(const char *fContent) {

}

int main(int argc, char *argv[]) {
	if(argc < 2) {
		fprintf(stderr, "Usage: %s filename.c\n", argv[0]);
		return 1;
	}

	const char *tFilename = argv[1];
	fprintf(stdout, "[INFO] Starting the tokenization of %s\n", tFilename);

	int fd = open(tFilename, O_RDWR | O_CREAT);
	char *buf;

	tokenize(buf);

	return 1;
}
