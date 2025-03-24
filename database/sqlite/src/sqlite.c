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
	(*ppDb)->zFileDescriptor = fd;
}

void sqlite_exec(sqlite *pDb, const char *zSqlCmd) {
	sqlite_stmt *stmt = (sqlite_stmt*)malloc(sizeof(sqlite_stmt));
	sqlite_prepare(zSqlCmd, pDb, &stmt);
}

void sqlite_prepare(const char *zSqlCmd, sqlite *pDb, sqlite_stmt **ppStmt) {
	
}
