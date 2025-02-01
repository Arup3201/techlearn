#include<stdio.h>

int printmsg(char*);

int main(int argc, char* argv[]) {
	char* message;
	if(argc != 2) {
		fprintf(stderr, "usage: %s <message>\n", argv[0]);
		return 1;	
	}

	if(!printmsg(message)) {
		fprintf(stderr, "%s: couldn't print message to console\n", argv[0]);
		return 1;
	}

	printf("Message delivered\n");

	return 0;
}

int printmsg(char* msg) {
	FILE *f;
	f = fopen("/dev/console", "w");
	if(f == (FILE*)NULL) {
		return 0;
	}

	fprintf(f, "%s\n", msg);
	fclose(f);

	return 1;
}
