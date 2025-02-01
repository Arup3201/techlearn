/* remote implementation of the printmsg
 * the procedure is named printmsg_1 as the unique identifier was 1
 */

#include<stdio.h>
#include "msg.h"

int* printmsg_1_svc(char** msg, struct svc_req *req) {
	static int result; // must be static
	FILE *f;
	f = fopen("/dev/console", "w");
	if(f == (FILE*)NULL) {
		result = 0;
		return &result;
	}

	fprintf(f, "%s\n", *msg);
	fclose(f);
	result = 1;
	return &result;
}
