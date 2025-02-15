// !/bin/gcc - local directory listing server

#include<stdio.h>

extern void read_dir(char*);

int main(int argc, char* argv[]) {
	if(argc != 2) {
		fprintf(stderr, "usage: %s host", argv[0]);
		return 0;
	}

	char* dirname = argv[1];
	read_dir(dirname);

	return 1;
}
