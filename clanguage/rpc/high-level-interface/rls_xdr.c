#include<rpc/rpc.h>
#include "rls.h"

bool_t xdr_dir(XDR* xdrsp, char* c) {
	return (xdr_string(xdrsp, &c, DIR_SIZE));
}
