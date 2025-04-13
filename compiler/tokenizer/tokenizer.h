#ifndef TOKENIZER_H
#define TOKENIZER_H

#define BUFFER_SIZE 2
#define SENTINEL '\0'

struct Tokenizer {
	int tFd; // file descriptor of the file we are tokenizing
	char buffers[2][BUFFER_SIZE+1]; // 2 buffer scheme 
	int active; // active buffer 
	char *lexemeBegin;
	char *forward;
};


enum TokenType {
	TOK_CREATE, 
	TOK_TABLE, 
	TOK_SELECT, 
	TOK_FROM, 
	TOK_WHERE, 
	TOK_ID, 
	TOK_SPACE
};

struct SymbolTable {
	enum TokenType tokenType;
	char tokenValue[128];
};

#endif
