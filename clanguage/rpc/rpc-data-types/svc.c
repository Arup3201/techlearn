#include <rpc/clnt_stat.h>
#include<rpc/rpc.h>
#include "simple.h"

int main() {	
	if(rpc_reg(SIMPLE_PROG, SIMPLE_VERS, SIMPLE_PROC, get_simple, (xdrproc_t)xdr_void, (xdrproc_t)xdr_simple, "visible") != -1) {
		fprintf(stderr, "Error: Server failed to register");
		return 0;
	}
	svc_run();
	fprintf(stderr, "Error: Server svc_run returned");
	return 1;
}
