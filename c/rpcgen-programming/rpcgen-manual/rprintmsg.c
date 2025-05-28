#include<stdio.h>
#include "msg.h"

int main(int argc, char* argv[]) {
	CLIENT* clnt;
	int *result;
	char* message;
	char* server;

	if(argc != 3) {
		/* if 3 arguments are not passed
		 * program server message
		 */
		fprintf(stderr, "usage: %s host message\n", argv[0]);
		return 1;
	}

	server = argv[1];
	message = argv[2];

	/* create connection between client and server
	 * pass the remote program name and version
	 */
	clnt = (CLIENT*)clnt_create(server, PRINTMSGPROG, PRINTMSGVERS, "visible");

	if(clnt == (CLIENT*)NULL) {
		/* failed to connect to server
		 * print error and die
		 */
		clnt_pcreateerror(server);
		return 1;
	}

	result = printmsg_1(&message, clnt);
	if(result == (int*)NULL) {
		/* failed to call the procedure
		 * print error and die
		 */
		clnt_perror(clnt, server);
		return 1;
	}

	if(*result == 0) {
		fprintf(stderr, "%s: couldn't print the message\n", argv[0]);
		return 1;
	}

	printf("Message delivered\n");
	clnt_destroy(clnt);

	return 0;
}
