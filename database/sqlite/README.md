# Database - SQLite 

Following theory is taken from the [YouTube video](https://www.youtube.com/watch?v=IrzF4r9hqlY). 

## SQLite Architecture

SQLite architecture can be divided into 2 parts -

- One part is responsible for understanding the sql command that we have written, and generete bytecode for that command 
- Another part will use this bytecode to process the data 

The first part will have 4 components -

1. Tokenizer: Commands will have some special parts inside them like `CREATE`, `INSERT`, `INTO` etc. The tokenizer will break the command into small tokens for further processing.
2. Parser: After we get the tokens from the command, we need to check whether they make sense together. If the tokens together actually yield to some result or is it just a wrong statement? Parser will help us do that. It will generate a parse tree from the tokens and from the parse tree we can understand whether it is a correct command or not syntactically.
3. Code Generator: Code generator will generate the byte code for the command. This bytecode will be used to process the data.

This part is also called the "frontend" of the SQLite database architecture. The other part is the "backend" that is responsible for data processing.

The "backend" has the following components -

1. Virtual Machine: Virtual machine or virtual database engine (VDBE in short) will get the bytecode and process the data. It is a big switch statement that decides whether to do select or insert or update based on the bytecode.
2. B/B+ Tree: Virtual machine will use B-Tree to get the data from the disk. SQLite stores every table and index in a seperate b-tree. To access a table or index we need the page number and b-tree will get that page for us.
3. Pager: Pager is responsible for making read/write operations on disk for pages. A database is stored in a disk as a single file. That file contains the database tables/indexes in the form of pages. It is an continuous array of pages. Every page is usually 4KB size nd they can be indexes using the page number. Pager also caches the recently accessed pages for faster operations. It also helps in ACID, locking management and rollback operation.
4. Virtual File System: VFS is used to make sure we are supporting cross-platform pperations. It is a common layer to support different operations depending on the type of platform we are in. It is also called OS interface because it defines the interface for different operations in different OS.

SQLite documentation on ["SQLite in 5 Minutes or Less"](https://sqlite.org/quickstart.html) explains how to use the SQLite C/C++ interface to build databases from C/C++ code.

In this interface, the following routines are important -

1. `sqlite3_open`: routine to open the database 
2. `sqlite3_exec`: routine to execute the SQL command against the database 
3. `sqlite3_close`: routine to close the database connection

Following code block uses these 3 routines to run the command against the database for output (src: SQLite documentation).

```c
#include <stdio.h>
#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
  int i;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

int main(int argc, char **argv){
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;

  if( argc!=3 ){
    fprintf(stderr, "Usage: %s DATABASE SQL-STATEMENT\n", argv[0]);
    return(1);
  }
  rc = sqlite3_open(argv[1], &db);
  if( rc ){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return(1);
  }
  rc = sqlite3_exec(db, argv[2], callback, 0, &zErrMsg);
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
  sqlite3_close(db);
  return 0;
}
```

Similar to the above program we need these 3 functions to execute SQL statements. In our case we will use `sqlite` object which is same as the `sqlite3` in the official docs.

For now I have the structure -

```c
typedef struct sqlite sqlite;

struct sqlite {
	char *zFilename;
	int zFileDescriptor;
};
```

We initialize the `sqlite` object using the `sqlite_open()` function. The function takes the database filename and opens it. It stores the file descriptor of the database file in the `sqlite` object.

```c
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
```

After the database connection is established, we need to execute the sqlite command with `sqlite_exec` routine. So let's understand what happens inside the `sqlite_exec` routine.

The execution starts first by tokenizing the sql text command. The tokenizer concept is discussed [here](../tokenizer/README.md).

In our SQLite example, we are going to support the following operations -

- CREATE 
- INSERT 
- UPDATE 
- DELETE
- SELECT 

Following are the syntax for the 5 operations in SQLite -

- `CREATE TABLE t1(a, b PRIMARY KEY);`, `CREATE TABLE t1(a, b UNIQUE);`. `PRIMARY KEY`, `UNIQUE`, `IF NOT EXIST`, `NULL`, and `NOT NULL` etc.
- `SELECT a, b, c FROM t1`, `SELECT a, b, sum(c) FROM t1 GROUP BY a`, `SELECT a, b, max(c) FROM t1 GROUP BY a` and `SELECT a, b, c FROM t1 ORDER BY a`. `sum`, `max`, `min`, `GROUP BY`, `ORDER BY` and `LIMIT`.

## CREATE Tokenize 


