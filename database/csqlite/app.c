#include<stdio.h>
#include "sqlite3.h"

int main(int argc, char* argv[]) {
	sqlite3* db = 0; // connection handle
	sqlite3_stmt* stmt = 0; // statement handle
	int retcode;
	char *db_name = "TEST_DB";

	retcode = sqlite3_open(db_name, &db);
	if(retcode != SQLITE_OK) {
		sqlite3_close(db);
		fprintf(stderr, "Could not open sqlite database %s\n", db_name);
		return retcode;
	}

	retcode = sqlite3_prepare(db, "SELECT * FROM users", -1, &stmt, 0);
	if(retcode != SQLITE_OK) {
		sqlite3_close(db);
		fprintf(stderr, "Could not prepare the statement");
		return retcode;
	}
	while(sqlite3_step(stmt) == SQLITE_ROW) {
		int i = sqlite3_column_int(stmt, 0);
		fprintf(stdout, "SID: %d", i);
	}

	sqlite3_finalize(stmt);
	sqlite3_close(db);

	return 0;
}
