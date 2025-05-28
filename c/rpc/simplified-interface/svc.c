#include <rpc/xdr.h>
#include<stdio.h>
#include<rpc/rpc.h>
#include<rpcsvc/rusers.h>

void* rusers();

int main() {
	if(rpc_reg(RUSERSPROG, 
			RUSERSVERS, 
			RUSERSPROG, 
			rusers, 
			(xdrproc_t)xdr_void, 
			(xdrproc_t)xdr_u_long, 
			"visible") == -1) {
		fprintf(stderr, "Couldn't register the remote procedure\n");
		return 1;
	}

	svc_run();
	fprintf(stderr, "Error: svc_run returned\n");
	return 0;
}
