#include <rpc/clnt.h>
#include<stdio.h>
#include "dir.h"

extern int errno;

int main(int argc, char* argv[]) {
	CLIENT *clnt;
	char* server;
	char* dirname;
	readdir_res* result;

	if (argc != 3) {
		fprintf(stderr, "usage: %s host directory", argv[0]);
		return 1;
	}

	server = argv[1];
	dirname = argv[2];

	clnt = clnt_create(server, DIRPROG, DIRVERS, "tcp");
	if(clnt == (CLIENT*)NULL) {
		clnt_pcreateerror(server);
		return 1;
	}

	result = readdir_1(&dirname, clnt);
	if(result == (readdir_res*)NULL) {
		clnt_perror(clnt, server);
		return 1;
	}

	if(result->errno != 0) {
		errno = result->errno;
		perror(dirname);
		return 1;
	}

	namelist nl;
	for(nl=result->readdir_res_u.list; nl != (namelist)NULL; nl=nl->next) {
		printf("%s\n", nl->name);
	}

	xdr_free(xdr_readdir_res, &result);
	clnt_destroy(clnt);

	return 0;
}
