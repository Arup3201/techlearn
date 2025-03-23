#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>

#include "sqlite.h"

void sqlite_open(const char *zFilename, sqlite **ppDb) {
	int fd;
	fd = open(zFilename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if(fd == -1) {
		(*ppDb) = NULL;
		return;
	}

	strcpy((*ppDb)->zFilename, zFilename);
	(*ppDb)->zFiledescriptor = fd;
}
