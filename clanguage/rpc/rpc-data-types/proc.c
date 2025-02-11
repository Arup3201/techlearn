#include<rpc/rpc.h>
#include "simple.h"

simple* get_simple(struct svc_req* req) {
	static simple simp;
	simp.a = 10;
	simp.b = 6;

	return &simp;
}
