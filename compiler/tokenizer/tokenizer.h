#ifndef TOKENIZER_H
#define TOKENIZER_H

#define BUFFER_SIZE 2
#define SENTINEL '\0'
#define MAX_LEXEME_LEN 1024
#define MAX_KEYWORD_LEN 1024
#define NUM_KEYWORDS sizeof(KEYWORDS)/(MAX_KEYWORD_LEN*sizeof(KEYWORDS[0]))

enum TokenType {
	TOK_KEYWORD, 
	TOK_ID,
	NUM_TOKEN_TYPES
};

struct SymbolTable {
	char lexeme[128];
	TokenType token;
};

struct Tokenizer {
	int tFd; // file descriptor of the file we are tokenizing
	char buffers[2][BUFFER_SIZE+1]; // 2 buffer scheme 
	int active; // active buffer 
	char *lexemeBegin;
	char *forward;
	SymbolTable *symbolTable;
};

const char KEYWORDS[][MAX_KEYWORD_LEN] = {"table", "create", "from", "select", "update", "delete"};

#endif
