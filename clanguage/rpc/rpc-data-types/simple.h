#ifndef XDR_SIMPLE_H
#define XDR_SIMPLE_H

#include<rpc/rpc.h>
#include<rpc/xdr.h>

#define SIMPLE_PROG 0x20000001
#define SIMPLE_VERS 1
#define SIMPLE_PROC 1

typedef struct simple {
	int a;
	short b;
} simple;

extern simple* get_simple(struct svc_req*);

#endif
