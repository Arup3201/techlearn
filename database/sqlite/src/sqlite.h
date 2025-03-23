#ifndef SQLITE_H
#define SQLITE_H

typedef struct sqlite sqlite;

struct sqlite {
	char *zFilename;
	int zFiledescriptor;
};

void sqlite_open(const char*, sqlite**);

#endif
