#include<stdlib.h>
#include<syslog.h>
#include<unistd.h>
#include<stdio.h>

#include "become_daemon.h"

int main(int argc, char* argv[]) {
	char* LOGNAME = "BECOME_DAEMON";

	int ret = become_daemon(0);
	if(ret) {
		syslog(LOG_USER | LOG_ERR, "become_daemon call failed!");
		closelog();
		return EXIT_FAILURE;
	}

	openlog(LOGNAME, LOG_PID, LOG_USER);
	syslog(LOG_USER | LOG_INFO, "starting");

	while(1) {
		sleep(60);
		syslog(LOG_USER | LOG_INFO, "running");
	}

	return 0;
}
