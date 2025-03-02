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

## Debugging and fixing bugs

Although the code works as expected for inputs, we will face some issues when we try to do any of the following -

1. If we try to put `username` with length equal to 32.
2. If we try to put `email` with length equal to 255.
3. If we try to put too long `username` or `email` then it should give us an error but it does not.
4. Because we are using `sscanf` - buffer overflow may happen and the input might be written somewhere else.

Let's tackle one at a time. Why the first 2 problem happen? It is because we are allowing exactly 32 or 255 character for `username` and `email`.
```c
#define USERNAME_MAX_SIZE 32
#define EMAIL_MAX_SIZE 255
typedef struct {
	int id;
	char username[USERNAME_MAX_SIZE+1];
	char email[EMAIL_MAX_SIZE+1];
} Row;
```

Now we have space to put the null character at the end, and it will not create any problem if the inputs are exactly same as their maximum possible length.

But it would be better if we can inform the user of our database about the fact that their `username` or `email` are too long if that is the case.

```c
CompileResult sqlite_compile_statement(InputBuffer *in, Statement *stat) {
	if(strncmp(in->buffer, "insert", 6) == 0) {
		stat->type = STATEMENT_INSERT;	

		char* insert_keyword = strtok(in->buffer, " ");
		char* id_string = strtok(NULL, " ");
		char* username_string = strtok(NULL, " ");
		char* email_string = strtok(NULL, " ");

		if(id_string == NULL || username_string == NULL || email_string == NULL) {
			fprintf(stderr, "Error: Invalid syntax for insert statement.\n");
			return COMPILE_SYNTAX_ERROR;
		}

		if(strlen(username_string) > USERNAME_MAX_SIZE) {
			fprintf(stderr, "Error: Invalid value found in insert statement.\n");
			return COMPILE_INPUT_ERROR;
		}

		if(strlen(email_string) > EMAIL_MAX_SIZE) {
			fprintf(stderr, "Error: Invalid value found in insert statement.\n");
			return COMPILE_INPUT_ERROR;
		}

		stat->row.id = atoi(id_string);
		strcpy(stat->row.username, username_string);
		strcpy(stat->row.email, email_string);

		return COMPILE_SUCCESS;
	}
	else if(strncmp(in->buffer, "select", 6) == 0) {
		stat->type = STATEMENT_SELECT;
		return COMPILE_SUCCESS;
	}
	
	return COMPILE_FAILURE;
}
```

`strtok` function will seperate the string using the delimeter passed as it's second argument. It might seem weird for the next 3 line. So, let me explain what is happening in case you are not familiar with `strtok` function.

The function uses a static copy of the string that we passed first time. Then it keeps this string and evertime we pass `NULL` to it, it uses previous string and returns the next token after seperating.

We also used another value `COMPILE_SYNTAX_ERROR` and another for string too long `COMPILE_INPUT_ERROR` to denote there is something with the input provided by the user.

```sh
sqlite> insert 1
Error: Invalid syntax for insert statement.
sqlite> insert 1 arup
Error: Invalid syntax for insert statement.
sqlite> insert 1 arup arup@thestartupcoder.com
Statement Executed.
sqlite> select
1 arup arup@thestartupcoder.com
Statement Executed.
```

## Persistence with disk

The current program can insert the rows and read them from disk when the program is running. But after we close the program the data is lost. We need to insert the rows again.

In this section, we will focus on how to store the data in the disk permanently and next time when we open it, we still can see the previously adds rows. Like SQLite we are going to persist the records by storing the database as a file.

Right now we are serializing the row into a memory block, which we consider as `page`. We can use the same method but instead of saving it into memory, we will save it into a file.

We will do all the operations with the page using a structure called `Pager`. Let's convert the code to use `Pager` now.

Our structure for `Table` will now become the following -

```c
#define TABLE_MAX_PAGES 100
typedef struct {
	int file_descriptor;
	unsigned int file_length;
	void* pages[TABLE_MAX_PAGES];
}Pager;

typedef struct {
	unsigned int n_rows;
	Pager* pager;
} Table;

```

Previosly we initialized the `Table` but now we need to do that to `Pager`. We need to -

- Open a file provided by the user. This file will be used for saving the database.
- Initialize the pager data structure and the table.

Let's write a seperate function that takes care of the pager initialization after opening the file provided.

```c
Pager* sqlite_init_pager(char* filename) {
	int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if(fd == -1) {
		fprintf(stderr, "Error: can't open file %s\n", filename);
		exit(EXIT_FAILURE);
	}
	off_t file_length = lseek(fd, 0, SEEK_END);
	
	Pager *p = (Pager*)malloc(sizeof(Pager));
	p->file_descriptor = fd;
	p->file_length = file_length;

	for(int i=0; i<TABLE_MAX_PAGES; i++) {
		p->pages[i] = NULL;
	}

	return p;
}
```

Next we will use this function to open our database (i.e. open the file where we are storing the database now) and then create a table working on this pager.

```c
Table* sqlite_open_db(char* filename) {
	Pager *p = sqlite_init_pager(filename);
	Table *table = (Table*)malloc(sizeof(Table));
	table->pager = p;
	table->n_rows = 0;
	return table;
}
```

Now, we have got the database started with the tables initialized. We need to also change the way we used to retrieve the pages. Now we will use pager to provide us the pages as we need it. Along with providing the pages, we are also adding caching. Caching here means the pages that we have tried to access after the program has been started will remain on the memory. When we need it we will get them without calculating where it is in the file. For the pages, we are trying to access for the first time - we will load them from the file.

```c
void* sqlite_get_page(Pager* pager, int page_no) {
	if(pager->pages[page_no] != NULL) {
		pager->pages[page_no] = malloc(PAGE_SIZE);

		ssize_t num_pages = pager->file_length / PAGE_SIZE;
		if(pager->file_length % PAGE_SIZE) {
			num_pages += 1;
		}

		if(page_no <= num_pages) {
			lseek(pager->file_descriptor, page_no * PAGE_SIZE, SEEK_SET);
			ssize_t nbytes = read(pager->file_descriptor, pager->pages[page_no], PAGE_SIZE);
			if(nbytes < 0) {
				fprintf(stdout, "Error: pager can't read the contents of the page\n");
				exit(EXIT_FAILURE);
			}
		}
	}

	return pager->pages[page_no];
}

```

We can get the page if it already exists in the memory otherwise we get the page from the file. Let's use this function get the page.

```c
void* get_table_row(Table *table, int row_no) {
	int page_no = row_no / ROWS_PER_PAGE;

	void *page = sqlite_get_page(table->pager, page_no);

	unsigned int row_offset = row_no % ROWS_PER_PAGE;
	unsigned int byte_offset = row_offset * ROW_SIZE;

	return page + byte_offset;
}
```

Now the import part of this section is how do we close the database and make sure the next time we visit the database we have all our previous changes with us?

For that we need to save the pages into the file before closing the pager. So, in this step we will do the following -

- write the pages to file
- free the pages
- free the table
- close the file that was opened by pager

```c
void sqlite_flush_page(Pager *pager, int page_no) {
	if(TABLE_MAX_PAGES >= page_no) {
		fprintf(stdout, "Error: trying to flush the page that does not exist\n");
		exit(EXIT_FAILURE);
	}

	if(pager->pages[page_no] == NULL) {
		return;
	}

	lseek(pager->file_descriptor, page_no * PAGE_SIZE, SEEK_SET);
	ssize_t nbytes = write(pager->file_descriptor, pager->pages[page_no], PAGE_SIZE);
	if(nbytes < 0) {
		fprintf(stderr, "Error: failed to write page to database file when flushing\n");
		exit(EXIT_FAILURE);
	}

	free(pager->pages[page_no]);
}

void sqlite_close_db(Table* table) {
	for(int i=0; i<TABLE_MAX_PAGES; i++) {
		sqlite_flush_page(table->pager, i);
	}

	close(table->pager->file_descriptor);
	free(table->pager);
	free(table);
}
```

Before running the new code, I want to create a `Makefile` that I can run to build an executable instead of writing the whole command everytime.

```make
main.out: main.c sqlite.c
	gcc -o main.out main.c sqlite.c -Wall -Wextra
```

I forgot to also make changes in the `main.c` file. In the `main.c` we are getting command like arguments for the filename.

```c
#include<stdbool.h>
#include<stdio.h>

#include "sqlite.h"

int main(int argc, char* argv[]) {
	InputBuffer *input = sqlite_new_input_buffer();

	if(argc != 2) {
		fprintf(stderr, "Error: needs a database file name\n");
		return 1;
	}

	char* filename = argv[1];
	Table *table = sqlite_open_db(filename);
	
	while(true) {
		sqlite_get_cmd(input);

		// if it is meta command - .exit, .tables
		if(input->buffer[0] == '.') {
			switch(sqlite_execute_meta_cmd(input, table)) {
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
	return 0;
}
```

Notice that we are also passing the `table` to `sqlite_execute_meta_cmd` so that at the time of `.exit` it will run the `sqlite_close_db`.

```c
MetaCmdResult sqlite_execute_meta_cmd(InputBuffer *in, Table *table) {
	if(strcmp(in->buffer, ".exit") == 0) {
		sqlite_close_db(table);
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

## Solving bugs in the persistence implementation

In the implementation there are some bugs -
Bug #1: After you insert some items, then exit the program. If you go again then you will not see anything.

Bug #1 happens because everytime we initialize the table `n_rows` to 0. And in the select statement we traverse till the `n_rows`. So when initializing the table we need to initialize it to number of rows.

```c
Table* sqlite_open_db(char* filename) {
	Pager *p = sqlite_init_pager(filename);
	Table *table = (Table*)malloc(sizeof(Table));
	table->pager = p;

	unsigned int nrows = p->file_length / ROW_SIZE;
	table->n_rows = nrows;
	return table;
}
```

Even if we do it, there is still a problem. Try to add some entries, run the program again and insert some entries. Next time when you try to see the rows, you will only see the entries you entered first time, all entries in the second run are gone.

Actually when you try to add a new row after reopening it next time, the new row is stored in the following way -

```sh
sqlite> select
1 a a@example.com
2 b b@example.com
0  
0  
0  
0  
0  
0  
0  
0  
0  
0  
0  
3 c c@example.com
```

It happens because after running the program first time, next time the table has `pager.file_length / ROW_SIZE` number of rows which is not 2 but something else. It happes because when we tried to save the rows at the time of flushing the pages, we needed to carefully save the rows according pages which are full and which are not.

So, we need to change the logic of the `sqlite_flush_pages` function for flushing full pages and partially filled pages.

```c
void sqlite_flush_page(Pager *pager, int page_no, size_t page_size) {
	if(page_no > TABLE_MAX_PAGES) {
		fprintf(stdout, "Error: trying to flush the page that does not exist\n");
		exit(EXIT_FAILURE);
	}

	if(pager->pages[page_no] == NULL) {
		return;
	}

	lseek(pager->file_descriptor, page_no * PAGE_SIZE, SEEK_SET);
	ssize_t nbytes = write(pager->file_descriptor, pager->pages[page_no], page_size);
	if(nbytes < 0) {
		fprintf(stderr, "Error: failed to write page to database file when flushing\n");
		exit(EXIT_FAILURE);
	}

	free(pager->pages[page_no]);
}

void sqlite_close_db(Table* table) {
	// flush the pages which are filled with rows
	unsigned int fully_filled_pages = table->n_rows / ROWS_PER_PAGE;
	for(int i=0; i<fully_filled_pages; i++) {
		sqlite_flush_page(table->pager, i, PAGE_SIZE);
	}

	// flush partially filled page 
	// we put the new rows in a page after previous page is filled
	// so we only have one partially filled page at the end
	size_t partial_page_size = (table->n_rows % ROWS_PER_PAGE) * ROW_SIZE;
	sqlite_flush_page(table->pager, fully_filled_pages, partial_page_size);

	close(table->pager->file_descriptor);
	free(table->pager);
	free(table);
}
```

Now it works as intended. Everytime we open the database it still contains the previous entries and we can also insert new entries and it works fine even then -

```sh
$ ./main.out test.db
sqlite> insert 1 a a@example.com
Statement Executed.
sqlite> select
1 a a@example.com
Statement Executed.
sqlite> .exit

$ ./main.out test.db
sqlite> select
1 a a@example.com
Statement Executed.
sqlite> insert 2 b b@example.com
Statement Executed.
sqlite> select
1 a a@example.com
2 b b@example.com
Statement Executed.
```

## Refactoring the code using cursor

To make the operations more easier, we are introducing `Cursor` object that helps us in the following -

- Delete a row pointer to by the cursor
- Insert new row using cursor 
- Modify the row pointer at by cursor
- Search the table using ID and point to the row using cursor 

We are going to do the following with cursor - 

- Create a cursor that points to the beginning of the table 
- Create a cursor that points to the end of the table 
- Cursor can advance one row at a time 
- Insert at end cursor
- Select current cursor which starts from first row and advance till end 

Let's start with selecting all the rows to see the table entries. We need a cursor that starts from the beginning. If the table contains no rows then the cursor is pointing at the end of the table otherwise it is not pointing at the end the table.

```c
Cursor* sqlite_get_start_cursor(Table *table) {
	Cursor *c = (Cursor*)malloc(sizeof(Cursor));
	c->table = table;
	c->row_num = 0;
	c->end_of_table = (table->n_rows == 0);

	return c;
}
```

To go to the next position in the table - we need to advance the cursor to the next row 

```c
void sqlite_cursor_advance(Cursor *c) {
	c->row_num += 1;
	if(c->row_num >= c->table->n_rows) c->end_of_table = true;
}
```

Now we can get the row pointed by the cursor instead of using the table and row no -

```c
void* sqlite_get_cursor_in_memory(Cursor *c) {
	int row_no = c->row_num;
	int page_no = row_no / ROWS_PER_PAGE;
	
	void *page = sqlite_get_page(c->table->pager, page_no);

	unsigned int row_offset = row_no % ROWS_PER_PAGE;
	unsigned int byte_offset = row_offset * ROW_SIZE;

	return page + byte_offset;
}
```

After we get the starting cursor, we can go through the table until the cursor is not pointing at the end of the table -

```c
StatementExecResult sqlite_execute_select_statement(Statement *stat, Table *table) {
	Row *r = (Row*)malloc(sizeof(Row));
	Cursor *c = sqlite_get_start_cursor(table);

	while(!(c->end_of_table)) {
		sqlite_deserialize(sqlite_get_cursor_in_memory(c), r);
		fprintf(stdout, "%d %s %s\n", r->id, r->username, r->email);
		sqlite_cursor_advance(c);
	}

	return STATEMENT_EXECUTION_SUCCESS;
}

```

Next we can use cursor for inserting at the end of the table. For adding at the end we need to create a cursor at the end like following -

```c
Cursor* sqlite_get_end_cursor(Table *table) {
	Cursor *c = (Cursor*)malloc(sizeof(Cursor));
	c->table = table;
	c->row_num = table->n_rows;
	c->end_of_table = true;

	return c;
}
```

Then we can use this end cursor to insert a row at the table -

```c
StatementExecResult sqlite_execute_insert_statement(Statement *stat, Table *table) {
	if(table->n_rows >= ROWS_PER_PAGE * TABLE_MAX_PAGES) {
		return STATEMENT_EXECUTION_TABLE_FULL;
	}
	
	Cursor *c = sqlite_get_end_cursor(table);
	sqlite_serialize(&(stat->row), sqlite_get_cursor_in_memory(c));
	table->n_rows += 1;
	return STATEMENT_EXECUTION_SUCCESS;
}
```

And as for the result... Everything works as it should -

```sh
$ ./main.out test.db
sqlite> select
Statement Executed.
sqlite> insert 1 a arup@example.com
Statement Executed.
sqlite> select
1 a arup@example.com
Statement Executed.
sqlite> insert 2 b bishal@example.com
Statement Executed.
sqlite> select
1 a arup@example.com
2 b bishal@example.com
Statement Executed.
sqlite> .exit

$ ./main.out test.db
sqlite> select
1 a arup@example.com
2 b bishal@example.com
Statement Executed.
sqlite> insert 3 c claint@example.com    
Statement Executed.
sqlite> select
1 a arup@example.com
2 b bishal@example.com
3 c claint@example.com
Statement Executed.
```

## Using B-Tree to manage the tables

B-Tree is a data structure used by SQLite to manage both table and indexes. Here's the reason why a B-Tree is a good data structure for database -

1. Searching for a particular value (logarithmic time)
2. Inserting and deleting a value that you already found is fast
3. Traversing a range of values is fast 

Following diagram represents a B-Tree -

![B-Tree Diagram](https://en.wikipedia.org/wiki/File:B-tree.svg)

Unlike binary tree where only 2 max children of every node, b-tree can have m children. To make a b-tree balanced every internal node should atleast have m/2 (upper limit) children.

Before we dive into implementing the SQLite using B-Tree let's understand some concepts of B-Tree and how to implement their operation so that we can build our table and indices in SQLite.

### Concepts related to b-tree

In a Binary Seach Tree (BST), one node can have at most 2 children and 1 value/key inside it. But in case of b-tree, there can be more than 2 children and there could be more than 1 key/value inside that node depending on the order (m) of the b-tree.

In BST, the leaf nodes can be on different levels - but in b-tree all the nodes will be in the same node. That is why it is also a balanced m order tree.

In b-tree following rules are followed - 

1. Every node can have max m children
2. Root node can have min 2 children, leaf node can have min 0 children, and internal node can have min ceil(m/2) children 
3. Every node can have max m-1 keys/values inside it 
4. Root node can have min 1 key, other nodes can have ceil(m/2)-1 keys/values in them 

Let's look at some of the concepts for inserting a node at a b-tree. It has some scenarios for when inserting a node -

