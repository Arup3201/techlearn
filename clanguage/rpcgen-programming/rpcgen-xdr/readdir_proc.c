#include<dirent.h>
#include "dir.h"

extern int errno;

readdir_res*
readdir_1_svc(nametype *dirname, struct svc_req* req) {
	static readdir_res res;

	DIR *dirp;

	dirp = opendir(*dirname);
	
	if(dirp == (DIR*) NULL) {
		res.error = errno;
		return &res;
	}

	xdr_free((xdrproc_t)xdr_readdir_res, &res);// free the memory allocated to previous res

	struct dirent *d;
	namelist *nlp;
	namelist nl;

	nlp = &res.readdir_res_u.list;
	while((d = readdir(dirp))) {
		nl = *nlp = (namenode*)malloc(sizeof(namenode));
		if(nl == (namenode*) NULL) {
			res.error = EAGAIN;
			closedir(dirp);
			return &res;
		}
		nl->name = strdup(d->d_name);
		nlp = &nl->next;
	}

	*nlp = (namelist)NULL;

	closedir(dirp);
	res.error = 0;
	return &res;
}
