#include<dirent.h>
#include "dir.h"

extern int errno;


readdir_res*
readdir_1_svc(nametype *dirname, struct svc_req* req) {
	static readdir_res res;

	DIR *dirp;

	dirp = opendir(*dirname);
	
	if(dirp == (DIR*) NULL) {
		res.errno = errno;
		return &res;
	}

	xdr_free(xdr_readdir_res, &res);// free the memory allocated to previous res

	struct dirent *d;
	namelist *nlp;
	namelist nl;

	nlp = &res.readdir_res_u.list;
	while(d == readdir(dirp)) {
		nl = *nlp = (namelist)malloc(sizeof(namelist));
		if(nl == (namelist) NULL) {
			res.errno = errno;
			closedir(dirp);
			return &res;
		}
		nl->name = strdup(d->d_name);
		nlp = &(nl->next);
	}

	*nlp = (namelist)NULL;

	closedir(dirp);
	res.errno = 0;
	return &res;
}
