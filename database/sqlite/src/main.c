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
