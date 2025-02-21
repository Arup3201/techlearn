
#include "sqlite.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

void sqlite_free_buffer(InputBuffer *in) {
	free(in->buffer);
	free(in);
}
