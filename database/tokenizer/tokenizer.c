#include<stdio.h>
#include<wchar.h>
#include<locale.h>
#include<fcntl.h>
#include<unistd.h>

#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
	if(argc < 2) {
		fprintf(stderr, "Usage: %s filename.c\n", argv[0]);
		return 1;
	}

	// enable unicode
	setlocale(LC_ALL, "");
	
	char ch;

	char *tFilename = argv[1];
	
	int fd = open(tFilename, O_RDONLY, S_IRUSR);
	char buffer[MAX_BUFFER_SIZE];
	read(fd, buffer, MAX_BUFFER_SIZE);

	int i=0;
	ch = buffer[i];
	while(ch != '\0') {
		fprintf(stdout, "%c -> u%04X\n", ch, (wchar_t)ch);
		ch = buffer[++i];
	}

	return 1;
}
