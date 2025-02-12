#include<stdio.h>
#include<dirent.h>
#include "rls.h"

char* read_dir(char* dirname) {
	DIR* dirp;
	struct dirent *d;

	dirp = opendir(dirname);
	if(dirp == NULL) {
		fprintf(stderr, "Error: Failed to find directory %s", dirname);
		return NULL;
	}

	static char dir[DIR_SIZE];
	sprintf(dir, "%s\n", dirname);
	while((d = readdir(dirp)) != NULL) {
		sprintf(dir, "%s\n", d->d_name);
	}

	closedir(dirp);
	return (char*)dir;
}
