#include<rpc/rpc.h>
#include "simple.h"

int main(int argc, char* argv[]) {
	if(argc != 2) {
		fprintf(stderr, "usage: %s host", argv[0]);
		return 0;
	}

	enum clnt_stat cs;
	simple out;
	cs = rpc_call(argv[1], SIMPLE_PROG, SIMPLE_VERS, SIMPLE_PROC, xdr_void, (char*)0, xdr_simple, (simple*)&out, "visible");
	if(cs != RPC_SUCCESS) {
		fprintf(stderr, "%s: Couldn't call the remote procedure", argv[1]);
		return 1;
	}

	fprintf(stderr, "%s: output - {%d, %d}", argv[1], out.a, out.b);
	return 1;
}
