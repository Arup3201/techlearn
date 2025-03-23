#include<stdio.h>
#include "sqlite.h"

int main(int argc, char *argv[]) {
	if(argc < 2) {
		fprintf(stderr, "Usage: %s DB_FILE\n", argv[0]);
		return 1;
	}

	char *db_filename = argv[1];
	sqlite *db;
	sqlite_open(db_filename, &db);
	if(db == NULL) {
		fprintf(stderr, "[ERROR] Can't open the database %s\n", db_filename);
		return 1;
	}
	
	// REPL Interface
	// while(1) {
	// 	cmd = ...;
	// 	res = sqlite_exec(db, cmd);

	// 	if(res is not OK) {
	// 		 error
	// 		 sqlite_close_db();
	// 		 return 1;
	// 	}
	// }

	// sqlite_close_db(db);

	return 0;
}
