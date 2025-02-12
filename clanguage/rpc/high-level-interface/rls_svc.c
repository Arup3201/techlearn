#include "rls.h"
#include <rpc/rpc.h>

int main() {
	extern bool_t xdr_dir(XDR*, char*);

	registerrpc(READDIR_PROG, READDIR_VERS, READDIR_PROC, read_dir, (xdrproc_t)xdr_dir, (xdrproc_t)xdr_dir);
	svc_run();

	return 1;
}
