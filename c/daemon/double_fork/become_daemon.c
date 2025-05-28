#include <fcntl.h> // open, close
#include<unistd.h> // fork, setsid, _exit, 
#include<stdlib.h> // EXIT_SUCCESS
#include<sys/stat.h> // umask
#include<stdio.h>

#include "become_daemon.h"

int become_daemon(int flags) {
	switch(fork()) {
		case -1: return -1; // fork failed so return
		case 0: break; // it is the child process so process to next code block
		default: _exit(EXIT_SUCCESS); // it is the parent
	}

	if(setsid() == -1) { // open a new session and make the current child process it's process group leader
		return -1;
	}

	switch(fork()) {
		case -1: return -1; // fork failure
		case 0: break; // child process 
		default: _exit(EXIT_SUCCESS); // exit the first child process
	}

	// after the parent process of the second child process is exited. the current process is now an orphan process.
	// init will become the parent process of this orphan process
	
	// according to the flag we will
	// change the directory to /
	// clear file creation mode mask
	// close all open files
	// set stdin, stdout and stderr to /dev/null
	
	if(!(flags & BD_NO_MASK)) umask(0);

	if(!(flags & BD_NO_CHDIR)) chdir("/");

	if(!(flags & BD_NO_OPEN_FILE)) {
		int maxfd;
		maxfd = sysconf(_SC_OPEN_MAX);

		if(maxfd == -1) {
			maxfd = BD_MAX_CLOSE;
		}

		int fd = maxfd;
		while(fd >= 0) {
			close(fd);
			fd --;
		}
	}

	if(!(flags & BD_NO_STD)) {
		close(STDIN_FILENO);

		int fd = open("/dev/null", O_RDWR);
		if(fd != STDIN_FILENO) {
			return -1;
		}
		if(dup2(STDIN_FILENO, STDOUT_FILENO) != STDIN_FILENO) {
			return -2;
		}
		if(dup2(STDIN_FILENO, STDERR_FILENO) != STDIN_FILENO) {
			return -3;
		}
	}

	return 0;
}
