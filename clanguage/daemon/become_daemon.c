#include<stdio.h>
#include<unistd.h>

int main(int argc, char* argv[]) {
	// directory - '/' which means no child directory
	int nochild = 0;
	
	// no input, output and error - means the file descriptor is closed
	int nofd = 0;

	if(daemon(nochild, nofd)) {
		perror("daemon");
		return 1;
	}

	// this daemon process stays for 60 secs
	sleep(60);

	return 0;
}
