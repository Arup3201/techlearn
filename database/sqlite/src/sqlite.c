#include <stdint.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<fcntl.h>
#include <sys/types.h>
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
const uint32_t LEAF_NODE_KEY_OFFSET = 0;
const uint32_t LEAF_NODE_VALUE_SIZE = ROW_SIZE;
const uint32_t LEAF_NODE_VALUE_OFFSET = LEAF_NODE_KEY_SIZE + LEAF_NODE_KEY_OFFSET;
const uint32_t LEAF_NODE_CELL_SIZE = LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE;
const uint32_t LEAF_NODE_CELL_SPACE = PAGE_SIZE - LEAF_NODE_HEADER_SIZE;
const uint32_t LEAF_NODE_MAX_CELLS = LEAF_NODE_CELL_SPACE / LEAF_NODE_CELL_SIZE;

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
	fprintf(stdout, "[INFO] Initializing pager from file %s\n", filename);

	int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if(fd == -1) {
		fprintf(stderr, "[Error] Can't open file %s\n", filename);
		exit(EXIT_FAILURE);
	}
	off_t file_length = lseek(fd, 0, SEEK_END);
	
	Pager *p = (Pager*)malloc(sizeof(Pager));
	p->file_descriptor = fd;
	p->file_length = file_length;
	p->num_pages = file_length / PAGE_SIZE;

	if(file_length % PAGE_SIZE) {
		fprintf(stdout, "[ERROR] Database file is corrupted!\n");
		exit(EXIT_FAILURE);
	}

	for(int i=0; i<TABLE_MAX_PAGES; i++) {
		p->pages[i] = NULL;
	}

	fprintf(stdout, "[INFO] Pager initialized\n");
	
	return p;
}

Table* sqlite_open_db(char* filename) {
	fprintf(stdout, "[INFO] Initializing the database\n");

	Pager *p = sqlite_init_pager(filename);
	Table *table = (Table*)malloc(sizeof(Table));
	table->pager = p;
	table->root_page_num = 0;

	if(p->num_pages == 0) {
		// database is new so root node is leaf node.
		fprintf(stdout, "[INFO] First page of the new database is initialized\n");

		void *page = sqlite_get_page(p, 0);
		initialize_leaf_node(page);
		fprintf(stdout, "[INFO] num_cells=%d\n", *(sqlite_leaf_node_num_cells(page)));
	}

	fprintf(stdout, "[INFO] Database sucessfully initialized\n");

	return table;
}

void sqlite_get_cmd(InputBuffer *in) {
	fprintf(stdout, "sqlite> ");

	int bytes_read;
	size_t buffer_length;
	
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
	

	fprintf(stderr, "[ERROR] Unrecognized command '%s'\n", in->buffer);
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

		strtok(in->buffer, " ");
		char* id_string = strtok(NULL, " ");
		char* username_string = strtok(NULL, " ");
		char* email_string = strtok(NULL, " ");

		if(id_string == NULL || username_string == NULL || email_string == NULL) {
			fprintf(stderr, "[ERROR] Invalid syntax for insert statement.\n");
			return COMPILE_SYNTAX_ERROR;
		}

		if(strlen(username_string) > USERNAME_MAX_SIZE) {
			fprintf(stderr, "[ERROR] Invalid value found in insert statement.\n");
			return COMPILE_INPUT_ERROR;
		}

		if(strlen(email_string) > EMAIL_MAX_SIZE) {
			fprintf(stderr, "[ERROR] Invalid value found in insert statement.\n");
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

void* sqlite_get_page(Pager* pager, uint32_t page_num) {

	if(page_num > TABLE_MAX_PAGES) {
		fprintf(stderr, "[ERROR] Trying to access out of bound page\n");
		exit(EXIT_FAILURE);
	}

	if(pager->pages[page_num] == NULL) {
		void *page = malloc(PAGE_SIZE);

		if(page_num <= pager->num_pages) {
			lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET);
			ssize_t nbytes = read(pager->file_descriptor, page, PAGE_SIZE);
			if(nbytes < 0) {
				fprintf(stdout, "[ERROR] Pager can't read the contents of the page\n");
				exit(EXIT_FAILURE);
			}
		}

		pager->pages[page_num] = page;

		if(page_num >= pager->num_pages) {
			pager->num_pages += 1;
		}
	}

	return pager->pages[page_num];
}

void* sqlite_get_row(Cursor *c) {
	int page_no = c->page_num;
	void *page = sqlite_get_page(c->table->pager, page_no);

	return sqlite_leaf_node_value(page, c->cell_num);
}

Cursor* sqlite_get_table_start(Table *table) {
	Cursor *c = (Cursor*)malloc(sizeof(Cursor));
	c->table = table;

	c->page_num = table->root_page_num;
	c->cell_num = 0;
	
	void *root_node = sqlite_get_page(table->pager, table->root_page_num);
	uint32_t num_cells = *(sqlite_leaf_node_num_cells(root_node));
	c->end_of_table = (num_cells == 0);

	return c;
}


Cursor* sqlite_get_table_end(Table *table) {
	Cursor *c = (Cursor*)malloc(sizeof(Cursor));
	c->table = table;
	c->page_num = table->root_page_num;
	
	void *root_node = sqlite_get_page(table->pager, table->root_page_num);
	uint32_t num_cells = *(sqlite_leaf_node_num_cells(root_node));
	c->cell_num = num_cells;

	c->end_of_table = true;

	return c;
}

void sqlite_cursor_advance(Cursor *c) {
	c->cell_num += 1;
	void *root_node = sqlite_get_page(c->table->pager, c->table->root_page_num);
	uint32_t num_cells = *(sqlite_leaf_node_num_cells(root_node));
	if(c->cell_num >= num_cells) c->end_of_table = true;
}

void btree_leaf_node_insert(Cursor *c, uint32_t key, Row *row) {
	void *page = sqlite_get_page(c->table->pager, c->page_num);
	uint32_t num_cells = *sqlite_leaf_node_num_cells(page);
	if(num_cells >= LEAF_NODE_MAX_CELLS) {
		fprintf(stdout, "[ERROR] Not impelemented leaf splitting when node is full\n");
		exit(EXIT_FAILURE);
	}

	if(c->cell_num <= num_cells) {
		for(uint32_t i=num_cells; i>c->cell_num; i--) {
			memcpy(sqlite_leaf_node_cell(page, i), sqlite_leaf_node_cell(page, i-1), LEAF_NODE_CELL_SIZE);
		}

		*(sqlite_leaf_node_key(page, c->cell_num)) = key;
		*(sqlite_leaf_node_num_cells(page)) += 1; 
		sqlite_serialize(row, sqlite_leaf_node_value(page, c->cell_num));
	}
}

StatementExecResult sqlite_execute_insert_statement(Statement *stat, Table *table) {
	void *page = sqlite_get_page(table->pager, table->root_page_num);
	if(*(sqlite_leaf_node_num_cells(page)) >= LEAF_NODE_MAX_CELLS) {
		return STATEMENT_EXECUTION_TABLE_FULL;
	}
	
	Cursor *c = sqlite_get_table_end(table);
	btree_leaf_node_insert(c, stat->row.id, &(stat->row));
	return STATEMENT_EXECUTION_SUCCESS;
}

StatementExecResult sqlite_execute_select_statement(Table *table) {
	Row *r = (Row*)malloc(sizeof(Row));
	Cursor *c = sqlite_get_table_start(table);

	while(!(c->end_of_table)) {
		sqlite_deserialize(sqlite_get_row(c), r);
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
			return sqlite_execute_select_statement(table);

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
	Pager *pager = table->pager;
	uint32_t num_pages = pager->num_pages;
	for(uint32_t i=0; i<num_pages; i++) {
		sqlite_flush_page(table->pager, i, PAGE_SIZE);
	}

	close(table->pager->file_descriptor);
	free(table->pager);
	free(table);
}
