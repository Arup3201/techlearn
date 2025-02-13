#include<stdio.h>
#include<dirent.h>
#include <string.h>
#include "rls.h"

char** read_dir(char *dirname) {
	DIR* dirp;
	struct dirent *d;
	fprintf(stdout, "Trying to search the directory %s\n", dirname);

	dirp = opendir(dirname);
	if(dirp == NULL) {
		fprintf(stderr, "Error: Failed to find directory %s", dirname);
		return NULL;
	}

	static char dir[DIR_SIZE];
	strcat(dir, dirname);
	strcat(dir, "\n\n");

	while((d = readdir(dirp)) != NULL) {
		strcat(dir, d->d_name);
		strcat(dir, "\n");
	}
	
	closedir(dirp);
	return (char**)&dir;
}
