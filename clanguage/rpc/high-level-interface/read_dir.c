#include<stdio.h>
#include<dirent.h>

int read_dir(char *dir) {
	DIR* dirp;
	struct dirent *d;
	fprintf(stdout, "Trying to search the directory %s\n", dir);

	dirp = opendir(dir);
	if(dirp == NULL) {
		fprintf(stderr, "Error: Failed to find directory %s", dir);
		return NULL;
	}

	dir[0] = NULL;
	while((d = readdir(dirp)) != NULL) {
		sprintf(dir, "%s%s\n", dir, d->d_name);
	}
	
	closedir(dirp);
	return (int)dir;
}
