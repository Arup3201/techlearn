# Database - SQLite (Theory and Code)

This folder explores about one of the databases sqlite using C. We will talk about the theories of databases and some concepts to build one using C language.

The sqlite architecture follows the pattern -

1. Tokenizer
2. Parser
3. Code Generator
4. Virtual Machine
5. B-Tree
6. Pager
7. OS Interface

A query goes through a chain of components to retrieve the data. The sqlite working could be divided into 2 parts - front-end and back-end.

The frontend consists of the following elements -

1. Tokenizer
2. Parser
3. Code Generator

The input to the frontend is the sql query and the output is virtual machine byte code (a compiled program that can run in the database).

The backend consists of -

1. Virtual Machine
2. B-Tree
3. Pager
4. Pager
5. OS Interface

The frontend output (byte codes) will be the input to the virtual machine. The virtual machine will use this byte code to perform operations on one or more tables or indexes. The tables and indexes are stored in a data structure called B-Tree.

Each B-Tree consists of many nodes. Every node is of length one page. B-Tree can store these pages to disk or retrieve them back by issuing commands to pager.

The pager takes commands to read/write pages of data. It keeps track of the offset in the database file where the read/write will happen. It also keeps track of the recently accessed page and decides when to save them back to disk.

The OS Interface is the layer that differs for different operating systems. This interface was built for portability between different OS. It defines the way OS specific tasks like opening, writing, reading or closing files in a disk will happen. SQLite has VFSes object that provides methods for such along with some other OS specific things like getting the current time, randomly generating numbers. Currently SQLite only supports VFSes for unix and windows systems.

## REPL - Read Evaluate Print Loop

When you type `sqlite3` you go inside a REPL where the program reads your commands and evaluate it and print the result. Then move to the next input command.

Something like the following -

```sh
> sqlite3

SQLite version 3.50.0 2025-02-19 13:05:33
Enter ".help" for usage hints.
Connected to a transient in-memory database.
Use ".open FILENAME" to reopen on a persistent database.
> create table users(id int, username varchar(255), email varchar(255));
> .tables
users
> .exit
~
```

We can build something similar with C but as we have no language parser to understand anythin - the only command we can try is `.exit` that will exit the process.

I am writing the code inside a file called `repl.c`.

```c
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>


int main(int argc, char* argv[]) {
	char* cmd;
	size_t buffer_length, total_chars_read;

	while(true) {
		fprintf(stdout, "db> ");
		total_chars_read = getline(&cmd, &buffer_length, stdin);

		if(total_chars_read < 0) {
			fprintf(stdout, "%s: failed to read from input stream\n", argv[0]);
			exit(EXIT_FAILURE);
		}

		cmd[total_chars_read - 1] = 0;

		if(strcmp(cmd, ".exit") == 0) {
			fprintf(stdout, "%s: exiting the REPL\n", argv[0]);
			exit(EXIT_SUCCESS);
		} else {
			fprintf(stdout, "%s: can't recognize the command\n", argv[0]);
		}
	}

	return 0;
}
```

The code logic is pretty simple -

1. We run an infinite loop which is the main concept of REPL.
2. Everytime we read a line from input stream using `getline`. This function will store the line into `*cmd` and also store the number of bytes it need `buffer_length`. The last argument is about stream we want to use for our input. This function returns a line which includes new line if included. So to get the right value we have to subtract 1 from the numbers of chars read from the input stream.
3. The input could be `.exit` or something else. If it is `.exit` then we successfully exit the program otherwise we show an error message.

For the sake of building sqlite we should also start creating an `sqlite.h` file that will contain all the declarations related to the sqlite structures, enum types and functions. Everytime we can include in any file where we want to use those types and functions.

I am creating the following 3 files -
```
- sqlite.h
- sqlite.c
- main.c
```

`sqlite.h` header file will contain the definitions of the types and functions. `sqlite.c` will contain the implementation of the functions declared in the header file. `main.c` will implement the sqlite using those functions and tyes.

To compile and run the sqlite - we need to do this -

```sh
gcc main.c sqlite.c -o main.out
./main.out
```

I will convert the already existing repl example into code that uses structures and enums which will make it easier to extend this application.

The first structure I will create is `InputBuffer` that holds the user input in `buffer` and it's length in `length`. The structure looks like this -

*sqlite.h*
```c
typedef struct {
	char* buffer;
	int length;
} InputBuffer;
```

The `length` in the `InputBuffer` is the length of the input without new line.

Then I will  convert the process of taking the input from the command line into a function called `sqlite_get_cmd` that takes a pointer to the `InputBuffer` and stores the input and it's length.

*sqlite.c*
```c
void sqlite_get_cmd(InputBuffer *in) {
	fprintf(stdout, "sqlite> ");

	size_t buffer_length, bytes_read;
	
	// return length is the bytes read, it does not include null but includes the new line if given
	bytes_read = getline(&in->buffer, &buffer_length, stdin);

	if(bytes_read < 0) {
		fprintf(stderr, "Error: failed to read from buffer\n");
		exit(EXIT_FAILURE);
	}

	in->length = bytes_read - 1;
	in->buffer[in->length] = '\0';
}
```

We can now use this function to get the input from the command line in the `main.c` file.

*main.c*
```c
int main(int argc, char* argv[]) {
	InputBuffer *input = (InputBuffer*)malloc(sizeof(InputBuffer));	

	while(true) {
		sqlite_get_cmd(input);

		// if it is meta command - .exit, .tables
		if(input->buffer[0] == '.') {
			switch(sqlite_execute_meta_cmd(input)) {
				case META_COMMAND_SUCCESS:
					continue;
				case META_COMMAND_FAILURE:
					fprintf(stderr, "Error: failed to execute the command '%s'\n", input->buffer);
					continue;
			}
		}

	}

	return 0;
}
```

Remember to always free up the memory allocated by `malloc`. For that I am writing another function `sqlite_free_buffer`.

*sqlite.c*
```c
void sqlite_free_buffer(InputBuffer *in) {
	free(in->buffer);
	free(in);
}
```

Make sure to include the definition in the header file.

*sqlite.h*
```c
void sqlite_get_cmd(InputBuffer*);
void sqlite_free_buffer(InputBuffer*);
```

Now I hope you have understood the flow of how I am writing the code and maintaining them between files.


## SQLite compiler and virtual machine

The main part of the sqlite frontend is the compiler that converts the sql statement into the internal representation supported to do the database work.

We will simplify the 2 parts - SQL Command Processor and Virtual Machine. SQL command processor has 3 main parts - tokenizer, parser and code generator. We will not use any complexity for the command processor. Our command processor for now will just understand "meta" commands and `insert` and `select` commands. "meta" commands are commands that are not considered part sql like `.exit`, `.tables` etc. Any command out of them will be unrecognized. 

We will break the command processor into 2 parts - one for meta commands and other for sql statements.

"meta" commands are commands that are not considered part sql like `.exit`, `.tables` etc.

Let's write the function that takes care of the meta commands (e.g. `.exit`, `.tables` etc).

```c
MetaCmdResult sqlite_execute_meta_cmd(InputBuffer *in) {
	if(strcmp(in->buffer, ".exit") == 0) {
		exit(EXIT_SUCCESS);
	}

	if(strcmp(in->buffer, ".tables") == 0) {
		fprintf(stdout, "Tables will be fetched...\n");
		return META_COMMAND_SUCCESS;
	}
	

	fprintf(stderr, "Unrecognized command '%s'\n", in->buffer);
	return META_COMMAND_FAILURE;
}
```

The function `sqlite_execute_meta_cmd` takes the pointer to the input buffer and returns either `META_COMMAND_FAILURE` or `META_COMMAND_SUCCESS` depending on whether the meta command was valid or not. We only handle 2 types of meta commands for now `.exit` and `.tables`. Other than these 2 commands any other command will be treated as unrecognized.

The types are defined in the header file.

```c
typedef enum {
	META_COMMAND_FAILURE, 
	META_COMMAND_SUCCESS
} MetaCmdResult;
```

Till now the overall repl program is completely converted and it looks something like this -

```c
#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>

#include "sqlite.h"

int main(int argc, char* argv[]) {
	InputBuffer *input = (InputBuffer*)malloc(sizeof(InputBuffer));	

	while(true) {
		sqlite_get_cmd(input);

		// if it is meta command - .exit, .tables
		if(input->buffer[0] == '.') {
			switch(sqlite_execute_meta_cmd(input)) {
				case META_COMMAND_SUCCESS:
					continue;
				case META_COMMAND_FAILURE:
					fprintf(stderr, "Error: failed to execute the command '%s'\n", input->buffer);
					continue;
			}
		}
	}

	sqlite_free_buffer(input);
	return 0;
}
```

We are using a `switch` statement to handle different outputs of the `sqlite_execute_meta_cmd` function. For success output, we continue otherwise we print an error and then proceed.

Now with this program we can handle some commands coming to us. More precisely the meta commands will be handled. To complete sql compiler part that will give us a format to work with the database, we also need to handle sql statements. They are the main ingredients of any database to talk to the database and get the data.

For now we will keep it simple. We will write a function `sqlite_execute_statement` that takes the pointer to input buffer and a `Statement` variable address. This function will decide based on the statement what type of statement it is. For now we handle `insert` and `select` type statement.

```c
CompileResult sqlite_compile_statement(InputBuffer *in, Statement *stat) {
	if(strncmp(in->buffer, "insert", 6) == 0) {
		stat->type = STATEMENT_INSERT;
		return COMPILE_SUCCESS;
	}
	else if(strncmp(in->buffer, "select", 6) == 0) {
		stat->type = STATEMENT_SELECT;
		return COMPILE_SUCCESS;
	}
	
	return COMPILE_FAILURE;
}
```

We can compare the input buffer with characters like `insert` and `select`. Depending on the match we either return `COMPILE_SUCCESS` if got some type otherwise return `COMPILE_FAILURE`.

The types are defined in the header file -

```c
typedef enum {
	STATEMENT_SELECT, 
	STATEMENT_INSERT, 
	STATEMENT_DELETE
} StatementType;

typedef struct {
	StatementType type;
} Statement;

typedef enum {
	COMPILE_FAILURE, 
	COMPILE_SUCCESS
} CompileResult;

CompileResult sqlite_compile_statement(InputBuffer*, Statement*);
```

After we compile the statement into format that we can understand, we can move to the virtual machine that will execute the format into an operation on database.

For that I am writing a function `sqlite_execute_statement` that takes the `Statement` address and process the statement for database operation. For now we will keep it simple and just print some simple message based on the type of statement.

```c
StatementExecResult sqlite_execute_statement(Statement *stat) {
	switch(stat->type) {
		case STATEMENT_INSERT:
			fprintf(stdout, "INSERT operation will be implemented later\n");
			return STATEMENT_EXECUTION_SUCCESS;

		case STATEMENT_SELECT:
			fprintf(stdout, "INSERT operation will be implemented later\n");
			return STATEMENT_EXECUTION_SUCCESS;

		default:
			return STATEMENT_EXECUTION_FAILURE;
	}
}
```

The types are defined like the following -

```c
typedef enum {
	STATEMENT_EXECUTION_FAILURE, 
	STATEMENT_EXECUTION_SUCCESS
} StatementExecResult;

StatementExecResult sqlite_execute_statement(Statement*);
```

The function returns `STATEMENT_EXECUTION_FAILURE` on detecting any statement that it does not recognize otherwise it returns `STATEMENT_EXECUTION_SUCCESS`.

After we have all the functions we can combine them in `main.c` like the following -

```c
#include<stdbool.h>
#include<stdlib.h>
#include<stdio.h>

#include "sqlite.h"

int main(int argc, char* argv[]) {
	InputBuffer *input = (InputBuffer*)malloc(sizeof(InputBuffer));	

	while(true) {
		sqlite_get_cmd(input);

		// if it is meta command - .exit, .tables
		if(input->buffer[0] == '.') {
			switch(sqlite_execute_meta_cmd(input)) {
				case META_COMMAND_SUCCESS:
					continue;
				case META_COMMAND_FAILURE:
					fprintf(stderr, "Error: failed to execute the command '%s'\n", input->buffer);
					continue;
			}
		}

		// if it is a sql statement
		Statement statement;
		switch(sqlite_compile_statement(input, &statement)) {
			case COMPILE_SUCCESS:
				break;
			case COMPILE_FAILURE:
				fprintf(stderr, "Error:  Unrecognized statement at the start of '%s'\n", input->buffer);
				continue;
		}

		// execute the sql statement
		switch(sqlite_execute_statement(&statement)) {
			case STATEMENT_EXECUTION_SUCCESS:
				fprintf(stdout, "Statement Executed.\n");
				break;
			case STATEMENT_EXECUTION_FAILURE:
				continue;

		}
	}

	sqlite_free_buffer(input);
	return 0;
}
```

## Simplest database example

For learning purpose I will stick to an in-memory hard coded database. The database will look something like this -

| Column    | Type        |
| --------  | ----------- |
| id        | int         |
| username  | varchar(32) |
| email     | varchar(256)|

The database has multiple types and it has a very simple structure. Our goal for now is to build this table and access the contents. The table contains pages and every page stores the rows present in the table.

The insert statement will look like this -
```
insert arup arup@thestartupcoder.com
```

We need to first take the input from command like. So, let's modify the `sqlite_compile_statement` function to support what we want. We need to change the `Statement` structure like following -
```c
#define USERNAME_MAX_SIZE 32
#define EMAIL_MAX_SIZE 256
typedef struct {
	StatementType type;
	int id;
	char username[USERNAME_MAX_SIZE];
	char email[EMAIL_MAX_SIZE];
} Statement;
```

And the function will change to  this -

```c
CompileResult sqlite_compile_statement(InputBuffer *in, Statement *stat) {
	if(strncmp(in->buffer, "insert", 6) == 0) {
		stat->type = STATEMENT_INSERT;	

		int args_to_insert = sscanf(in->buffer, "insert %d %s %s", &stat->id, stat->username, stat->email);
		if(args_to_insert != 3) {
			return COMPILE_FAILURE;
		}

		return COMPILE_SUCCESS;
	}
	else if(strncmp(in->buffer, "select", 6) == 0) {
		stat->type = STATEMENT_SELECT;
		return COMPILE_SUCCESS;
	}
	
	return COMPILE_FAILURE;
}
```

I am using `sscanf` for formatted input as out statement has a format, and we know where our values are in that statement. Just like `scanf` it also returns the no of characters read. If the characters read is not equal to 3 then there is syntax error in the statement.

Let's look at how we are going to execute the insert and select statements.

When we execute the statement what we want to do. We will store the data in our table. But where is the table? Let's build one.

Table will be a structure... but what are the members of these structure? A table has pages. So one member will be pages. Other data that we might need is the number of rows saved in the table so that when inserting a new row, we know where to insert it.

```c
#define TABLE_MAX_PAGES 100
typedef struct {
	unsigned int n_rows;
	void* pages[TABLE_MAX_PAGES];
} Table;
```

We have the `Table` which contains the pages. Because page is just a block of memory and we are storing the rows in it, we need to get some size and offset values so that memory access becomes easier.

We need to understand the size of each entry inside a row and their offset i.e where they start from in the memory if we access that row. Every column size can be calculated using `get_attribute_size` macro.

```c
#define get_attribute_size(Struct, Attribute) (sizeof(((Struct*)0)->Attribute))
```

NOTE: For me the macro was a little tricky but after looking at stackoverflow, I understood it is actually simply accessing a member of a structure and calculating it's size. By doing `(Struct*))` we are creating null pointer and accessing the member `Attribute` from it. It does not create any object in memory and let's us calculate the size of the attribute.

Next we will calculate the size and offset of each member of the row. For that we need some constant size and offset calculations -

```c
const unsigned int ID_SIZE = get_attribute_size(Statement, id);
const unsigned int USERNAME_SIZE = get_attribute_size(Statement, username);
const unsigned int EMAIL_SIZE = get_attribute_size(Statement, email);

const unsigned int ID_OFFSET = 0;
const unsigned int USERNAME_OFFSET = ID_SIZE + ID_OFFSET;
const unsigned int EMAIL_OFFSET = USERNAME_SIZE + USERNAME_OFFSET;
```

Offset values are for accessing individual members in a row. 

We will use a `Row` structure to denote each row in the table. Each row contain the columns `id`, `username` and `email`.

```c
#define USERNAME_MAX_SIZE 32
#define EMAIL_MAX_SIZE 256
typedef struct {
	int id;
	char username[USERNAME_MAX_SIZE];
	char email[EMAIL_MAX_SIZE];
} Row;

typedef struct {
	StatementType type;
	Row* row;
} Statement;
```

For implementing the `insert` and `select` statement execution, we will first need to store the rows in a page. For that we need to serialize them into a block of memory which are pages. In the time of accessing the rows, we also need to deserialize them.

```c
void serialize(Row* source, void* destination) {
	memcpy(destination+ID_OFFSET, &(source->id), ID_SIZE);
	memcpy(destination+USERNAME_OFFSET, source->username, USERNAME_SIZE);
	memcpy(destination+EMAIL_OFFSET, source->email, EMAIL_SIZE);
}

void deserialize(void* source, Row* destination) {
	memcpy(&(destination->id), source+ID_OFFSET, ID_SIZE);
	memcpy(destination->username, source+USERNAME_OFFSET, USERNAME_SIZE);
	memcpy(destination->email, source+EMAIL_OFFSET, EMAIL_SIZE);
}
```

Now we need to know where to read/write the row. Here we are working with a table that loads the pages when we need it. It does not store it inside any variable. But fortunately, we have stored it in the memory through serialize, so as long as we know where the memory is - we can access it and store it inside the table page.

```c
void* get_table_row(Table *table, int row_no) {
	int page_no = row_no / ROWS_PER_PAGE;

	void *page = table->pages[page_no];
	if(page == NULL) {
		page = table->pages[page_no] = malloc(PAGE_SIZE);
	}

	unsigned int row_offset = row_no % ROWS_PER_PAGE;
	unsigned int byte_offset = row_offset * ROW_SIZE;

	return page + byte_offset;
}
```

This function will return the memory block starting address which can then be deserialized to get the `Row` object. Look at the code carefully and you will see that we are storing the row inside the table on the fly when we try to access it for the first time.

With this we can now process the insert statement in the following way -

```c
StatementExecResult sqlite_execute_insert_statement(Statement *stat, Table *table) {
	if(table->n_rows >= ROWS_PER_PAGE * TABLE_MAX_PAGES) {
		return STATEMENT_EXECUTION_TABLE_FULL;
	}
	
	serialize(&(stat->row), get_table_row(table, table->n_rows));
	table->n_rows += 1;
	return STATEMENT_EXECUTION_SUCCESS;
}
```

We have added one more type of `StatementExecResult` to denote table full error. Every insert will increase the table number of rows.

We can also make the select statement which is easier than insert statement -

```c
StatementExecResult sqlite_execute_select_statement(Statement *stat, Table *table) {
	Row *r;
	for(int i=0; i<table->n_rows; i++) {
		deserialize(get_table_row(table, i), r);
		fprintf(stdout, "%d %s %s\n", r->id, r->username, r->email);
	}

	return STATEMENT_EXECUTION_SUCCESS;
}
```

We just traverse all the rows from 0. Deserialize the row and print it.

Now we just have to put all of them in the `main.c` file together. We also need to create objects for `Table` in the memory. So for that let's quickly create some functions that will help us make new `InputBuffer` and `Table`.

```c
InputBuffer* sqlite_new_input_buffer() {
	InputBuffer* in = (InputBuffer*)malloc(sizeof(InputBuffer));
	in->buffer = (char*)malloc(MAX_INPUT_LENGTH);
	return in;
}

Table* sqlite_new_table() {
	Table *table = (Table*)malloc(sizeof(Table));
	for(int i=0; i<TABLE_MAX_PAGES; i++) {
		table->pages[i] = NULL;
	}
	table->n_rows = 0;
	return table;
}
```

After everything together, we have the following function that will let us insert entries in our db and also show us the entries using select.

```c
#include<stdbool.h>
#include<stdio.h>

#include "sqlite.h"

int main(int argc, char* argv[]) {
	InputBuffer *input = sqlite_new_input_buffer();
	Table *table = sqlite_new_table();
	
	while(true) {
		sqlite_get_cmd(input);

		// if it is meta command - .exit, .tables
		if(input->buffer[0] == '.') {
			switch(sqlite_execute_meta_cmd(input)) {
				case META_COMMAND_SUCCESS:
					continue;
				case META_COMMAND_FAILURE:
					fprintf(stderr, "Error: failed to execute the command '%s'\n", input->buffer);
					continue;
			}
		}

		// if it is a sql statement
		Statement statement;
		switch(sqlite_compile_statement(input, &statement)) {
			case COMPILE_SUCCESS:
				break;
			case COMPILE_FAILURE:
				fprintf(stderr, "Error:  Unrecognized statement at the start of '%s'\n", input->buffer);
				continue;
		}

		// execute the sql Statement
		switch(sqlite_execute_statement(&statement, table)) {
			case STATEMENT_EXECUTION_SUCCESS:
				fprintf(stdout, "Statement Executed.\n");
				break;

			case STATEMENT_EXECUTION_TABLE_FULL:
				fprintf(stdout, "Table is full.\n");
				break;

			case STATEMENT_EXECUTION_FAILURE:
				continue;

		}
	}

	sqlite_free_buffer(input);
	sqlite_free_table(table);
	return 0;
}
```

The output of the function will be like this -

```sh
sqlite> insert 1 arup arup@thestartupcoder.com
Statement Executed.
sqlite> insert 2 surendar surendar@email.com
Statement Executed.
sqlite> select
1 arup arup@thestartupcoder.com
2 surendar surendar@email.com
Statement Executed.
sqlite> .exit
```
