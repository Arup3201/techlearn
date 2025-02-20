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
