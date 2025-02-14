// rls.c - remote procedure call client

#include "rls.h"
#include <rpc/rpc.h>
#include <string.h>

void read_dir_clnt(char*, char*);

int main(int argc, char* argv[]) {
	if(argc != 3) {
		fprintf(stderr, "usage: %s host dirname", argv[0]);
		return 0;
	}

	char dir[dir_size];
	strcpy(dir, argv[2]);

	read_dir_clnt(argv[1], dir);
	printf("%s", dir);

	return 1;
}

void read_dir_clnt(char* host, char* dir) {
	extern bool_t xdr_dir(xdr*, char*);
	fprintf(stdout, "trying to search %s directory\n", dir);

	enum clnt_stat cs;
	cs = callrpc(host, readdir_prog, readdir_vers, readdir_proc, (xdrproc_t)xdr_dir, dir, (xdrproc_t)xdr_dir, dir);
	if(cs != 0) {
		clnt_perrno(cs);
		return;
	}
}

