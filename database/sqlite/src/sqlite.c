#include <stdint.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<inttypes.h>

#include "sqlite.h"

const unsigned int ID_SIZE = get_attribute_size(Row, id);
const unsigned int USERNAME_SIZE = get_attribute_size(Row, username);
const unsigned int EMAIL_SIZE = get_attribute_size(Row, email);

const unsigned int ID_OFFSET = 0;
const unsigned int USERNAME_OFFSET = ID_SIZE + ID_OFFSET;
const unsigned int EMAIL_OFFSET = USERNAME_SIZE + USERNAME_OFFSET;

const unsigned int ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

// common node headers
const uint32_t NODE_TYPE_SIZE = sizeof(uint8_t);
const uint32_t NODE_TYPE_OFFSET = 0;
const uint32_t IS_ROOT_SIZE = sizeof(uint8_t);
const uint32_t IS_ROOT_OFFSET = NODE_TYPE_OFFSET + NODE_TYPE_SIZE;
const uint32_t PARENT_POINTER_SIZE = sizeof(uint32_t);
const uint32_t PARENT_POINTER_OFFSET = IS_ROOT_OFFSET + IS_ROOT_SIZE;
const uint32_t COMMON_NODE_HEADER_SIZE = NODE_TYPE_SIZE + IS_ROOT_SIZE + PARENT_POINTER_SIZE;

// lead node headers
const uint32_t LEAF_NODE_NUM_CELLS_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_NUM_CELLS_OFFSET = COMMON_NODE_HEADER_SIZE + 0;
const uint32_t LEAF_NODE_HEADER_SIZE = COMMON_NODE_HEADER_SIZE + LEAF_NODE_NUM_CELLS_SIZE;

// leaf node body
const uint32_t LEAF_NODE_KEY_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_KEY_OFFSET = LEAF_NODE_HEADER_SIZE;
const uint32_t LEAF_NODE_VALUE_SIZE = ROW_SIZE;
const uint32_t LEAF_NODE_VALUE_OFFSET = LEAF_NODE_KEY_SIZE + LEAF_NODE_KEY_OFFSET;
const uint32_t LEAF_NODE_CELL_SIZE = LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE;
const uint32_t LEAF_NODE_CELL_SPACE = PAGE_SIZE - LEAF_NODE_HEADER_SIZE;
const uint32_t LEAF_NODE_MAX_CELLS = LEAF_NODE_CELL_SPACE / LEAF_NODE_CELL_SIZE;

// get and set functions for leaf node fields
uint32_t* sqlite_leaf_node_num_cells(void *node) {
	return node + LEAF_NODE_NUM_CELLS_OFFSET;
}
void* sqlite_leaf_node_cell(void *node, uint32_t cell) {
	return node + LEAF_NODE_KEY_OFFSET + LEAF_NODE_CELL_SIZE*cell;
}
uint32_t* sqlite_leaf_node_key(void *node, uint32_t cell) {
	return sqlite_leaf_node_cell(node, cell);
}
void* sqlite_leaf_node_value(void *node, uint32_t cell) {
	return sqlite_leaf_node_cell(node, cell) + LEAF_NODE_VALUE_OFFSET;
}
void initialize_leaf_node(void *node) {
	*(sqlite_leaf_node_num_cells(node)) = 0;
}

InputBuffer* sqlite_new_input_buffer() {
	InputBuffer* in = (InputBuffer*)malloc(sizeof(InputBuffer));
	in->buffer = (char*)malloc(MAX_INPUT_LENGTH);
	return in;
}

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

Table* sqlite_open_db(char* filename) {
	Pager *p = sqlite_init_pager(filename);
	Table *table = (Table*)malloc(sizeof(Table));
	table->pager = p;

	unsigned int nrows = p->file_length / ROW_SIZE;
	table->n_rows = nrows;
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

CompileResult sqlite_compile_statement(InputBuffer *in, Statement *stat) {
	if(strncmp(in->buffer, "insert", 6) == 0) {
		stat->type = STATEMENT_INSERT;	

		// buffer overflow problem with sscanf
		// int args_to_insert = sscanf(in->buffer, "insert %d %s %s", &(stat->row.id), stat->row.username, stat->row.email);
		// if(args_to_insert != 3) {
		// 	return COMPILE_FAILURE;
		// }

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

void sqlite_serialize(Row* source, void* destination) {
	memcpy(destination+ID_OFFSET, &source->id, ID_SIZE);
	memcpy(destination+USERNAME_OFFSET, source->username, USERNAME_SIZE);
	memcpy(destination+EMAIL_OFFSET, source->email, EMAIL_SIZE);
}

void sqlite_deserialize(void* source, Row* destination) {
	memcpy(&(destination->id), source+ID_OFFSET, ID_SIZE);
	memcpy(destination->username, source+USERNAME_OFFSET, USERNAME_SIZE);
	memcpy(destination->email, source+EMAIL_OFFSET, EMAIL_SIZE);
}

void* sqlite_get_page(Pager* pager, int page_no) {

	if(page_no > TABLE_MAX_PAGES) {
		fprintf(stderr, "Error: trying to access out of bound page\n");
		exit(EXIT_FAILURE);
	}

	if(pager->pages[page_no] == NULL) {
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

void* sqlite_get_cursor_in_memory(Cursor *c) {
	int row_no = c->row_num;
	int page_no = row_no / ROWS_PER_PAGE;
	
	void *page = sqlite_get_page(c->table->pager, page_no);

	unsigned int row_offset = row_no % ROWS_PER_PAGE;
	unsigned int byte_offset = row_offset * ROW_SIZE;

	return page + byte_offset;
}

Cursor* sqlite_get_end_cursor(Table *table) {
	Cursor *c = (Cursor*)malloc(sizeof(Cursor));
	c->table = table;
	c->row_num = table->n_rows;
	c->end_of_table = true;

	return c;
}

StatementExecResult sqlite_execute_insert_statement(Statement *stat, Table *table) {
	if(table->n_rows >= ROWS_PER_PAGE * TABLE_MAX_PAGES) {
		return STATEMENT_EXECUTION_TABLE_FULL;
	}
	
	Cursor *c = sqlite_get_end_cursor(table);
	sqlite_serialize(&(stat->row), sqlite_get_cursor_in_memory(c));
	table->n_rows += 1;
	return STATEMENT_EXECUTION_SUCCESS;
}

Cursor* sqlite_get_start_cursor(Table *table) {
	Cursor *c = (Cursor*)malloc(sizeof(Cursor));
	c->table = table;
	c->row_num = 0;
	c->end_of_table = (table->n_rows == 0);

	return c;
}


void sqlite_cursor_advance(Cursor *c) {
	c->row_num += 1;
	if(c->row_num >= c->table->n_rows) c->end_of_table = true;
}

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
