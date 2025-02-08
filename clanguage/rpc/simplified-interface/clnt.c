#include<stdio.h>
#include<rpc/rpc.h>
#include<rpcsvc/rusers.h>

int main(int argc, char* argv[]) {
	unsigned long nusers;
	enum clnt_stat cs;

	if(argc != 2) {
		fprintf(stderr, "usage: %s host", argv[0]);
		return 0;
	}

	if((cs = rpc_call(
					argv[1], // host server
					RUSERSPROG, // program number of ruser program
					RUSERSVERS,  // version number of ruser procedure
					RUSERSPROC_NUM, // procedure number of ruser remote procedure
					(xdrproc_t)xdr_void, // no input args so void
					(char*)0, // no input so 0
					(xdrproc_t)xdr_u_long, // input arg type encoder
					(char*)&nusers, // input address
					"visible" // transport type
			 )) != RPC_SUCCESS) {
		clnt_perrno(cs);
		return 0;
	}

	fprintf(stderr, "%ld users at %s", nusers, argv[1]);
	return 1;
}
