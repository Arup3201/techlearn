#include<rpc/xdr.h>
#include "simple.h"


bool_t xdr_simple(XDR* xdrsp, simple* simp) {
	if(!xdr_int(xdrsp, &simp->a)) return FALSE;
	if(!xdr_short(xdrsp, &simp->b)) return FALSE;

	return TRUE;
}
