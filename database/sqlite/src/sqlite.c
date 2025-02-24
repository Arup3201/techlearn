
#include "sqlite.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


const unsigned int ID_SIZE = get_attribute_size(Row, id);
const unsigned int USERNAME_SIZE = get_attribute_size(Row, username);
const unsigned int EMAIL_SIZE = get_attribute_size(Row, email);

const unsigned int ID_OFFSET = 0;
const unsigned int USERNAME_OFFSET = ID_SIZE + ID_OFFSET;
const unsigned int EMAIL_OFFSET = USERNAME_SIZE + USERNAME_OFFSET;

const unsigned int ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

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

		int args_to_insert = sscanf(in->buffer, "insert %d %s %s", &(stat->row.id), stat->row.username, stat->row.email);
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

void serialize(Row* source, void* destination) {
	memcpy(destination+ID_OFFSET, &source->id, ID_SIZE);
	memcpy(destination+USERNAME_OFFSET, source->username, USERNAME_SIZE);
	memcpy(destination+EMAIL_OFFSET, source->email, EMAIL_SIZE);
}

void deserialize(void* source, Row* destination) {
	memcpy(&(destination->id), source+ID_OFFSET, ID_SIZE);
	memcpy(destination->username, source+USERNAME_OFFSET, USERNAME_SIZE);
	memcpy(destination->email, source+EMAIL_OFFSET, EMAIL_SIZE);
}

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

StatementExecResult sqlite_execute_insert_statement(Statement *stat, Table *table) {
	if(table->n_rows >= ROWS_PER_PAGE * TABLE_MAX_PAGES) {
		return STATEMENT_EXECUTION_TABLE_FULL;
	}
	
	serialize(&(stat->row), get_table_row(table, table->n_rows));
	table->n_rows += 1;
	return STATEMENT_EXECUTION_SUCCESS;
}

StatementExecResult sqlite_execute_select_statement(Statement *stat, Table *table) {
	Row *r = (Row*)malloc(sizeof(Row));
	for(int i=0; i<table->n_rows; i++) {
		deserialize(get_table_row(table, i), r);
		fprintf(stdout, "%d %s %s\n", r->id, r->username, r->email);
	}

	return STATEMENT_EXECUTION_SUCCESS;
}

StatementExecResult sqlite_execute_statement(Statement *stat, Table *table) {
	switch(stat->type) {
		case STATEMENT_INSERT:
			return sqlite_execute_insert_statement(stat, table);

		case STATEMENT_SELECT:
			return sqlite_execute_select_statement(stat, table);

		default:
			return STATEMENT_EXECUTION_FAILURE;
	}
}


void sqlite_free_buffer(InputBuffer *in) {
	free(in->buffer);
	free(in);
}

void sqlite_free_table(Table* table) {
	for(int i=0; i<TABLE_MAX_PAGES; i++) {
		free(table->pages[i]);
	}
	free(table);
}
