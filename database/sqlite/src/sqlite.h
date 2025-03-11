#ifndef SQLITE_H
#define SQLITE_H

#ifndef __STDBOOL_H
#include<stdbool.h>
#endif

#ifndef __CLANG_STDINT_H
#include<stdint.h>
#endif

#define MAX_INPUT_LENGTH 1024
typedef struct {
	char* buffer;
	int length;
} InputBuffer;


typedef enum {
	META_COMMAND_FAILURE, 
	META_COMMAND_SUCCESS
} MetaCmdResult;

typedef enum {
	STATEMENT_SELECT, 
	STATEMENT_INSERT, 
	STATEMENT_DELETE
} StatementType;

#define USERNAME_MAX_SIZE 32
#define EMAIL_MAX_SIZE 255
typedef struct {
	int id;
	char username[USERNAME_MAX_SIZE+1];
	char email[EMAIL_MAX_SIZE+1];
} Row;

typedef struct {
	StatementType type;
	Row row;
} Statement;


typedef enum {
	COMPILE_FAILURE, 
	COMPILE_INPUT_ERROR, 
	COMPILE_SYNTAX_ERROR,
	COMPILE_SUCCESS
} CompileResult;

typedef enum {
	STATEMENT_EXECUTION_FAILURE,
	STATEMENT_EXECUTION_TABLE_FULL, 
	STATEMENT_EXECUTION_SUCCESS
} StatementExecResult;

#define get_attribute_size(Struct, Attribute) (sizeof(((Struct*)0)->Attribute))

#define TABLE_MAX_PAGES 100
typedef struct {
	int file_descriptor;
	unsigned int file_length;
	void* pages[TABLE_MAX_PAGES];
	uint32_t num_pages;
}Pager;

typedef struct {
	uint32_t root_page_num;
	Pager* pager;
} Table;

typedef struct {
	Table *table;
	uint32_t page_num;
	uint32_t cell_num;
	bool end_of_table;
} Cursor;

#define PAGE_SIZE 4096
#define ROWS_PER_PAGE 100

InputBuffer* sqlite_new_input_buffer();
Pager* sqlite_init_pager(char*); // takes a filename and returns a Pager pointer after initilaizing it
void* sqlite_get_page(Pager*, int);
Table* sqlite_open_db(char*);
void sqlite_get_cmd(InputBuffer*);
void sqlite_free_buffer(InputBuffer*);
void sqlite_close_db(Table*);
MetaCmdResult sqlite_execute_meta_cmd(InputBuffer*, Table*);
CompileResult sqlite_compile_statement(InputBuffer*, Statement*);
StatementExecResult sqlite_execute_statement(Statement*, Table*);

#endif
