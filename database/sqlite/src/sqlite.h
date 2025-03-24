#ifndef SQLITE_H
#define SQLITE_H

typedef struct sqlite sqlite;
struct sqlite {
	char *zFilename;
	int zFileDescriptor;
};

typedef struct sqlite_stmt sqlite_stmt;
struct sqlite_stmt {
	char *zSqlCmd;
	char **zCmdTokens;
};

void sqlite_open(const char*, sqlite**);
void sqlite_prepare(const char*, sqlite*, sqlite_stmt**);

#endif
