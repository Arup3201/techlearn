#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdbool.h>
#include<stdlib.h>

#include "tokenizer.h"

typedef struct Tokenizer Tokenizer;
typedef enum TokenType TokenType;

void initTokenizer(Tokenizer *t, char *fname) {
	Tokenizer *tokenizer = (Tokenizer*)malloc(sizeof(Tokenizer));
	tokenizer->tFd = open(fname, O_RDONLY, S_IRUSR);
	tokenizer->active = 0;
	read(tokenizer->tFd, tokenizer->buffers[tokenizer->active], BUFFER_SIZE);
	tokenizer->lexemeBegin = tokenizer->buffers[tokenizer->active];
	tokenizer->forward = tokenizer->lexemeBegin;
}

void destroyTokenizer(Tokenizer *t) {
	close(t->tFd);
	t->lexemeBegin = NULL;
	t->forward = NULL;
	free(t);
}

bool isSpace(char ch) {
	return ch == ' ';
}

int main(int argc, char* argv[]) {
	if(argc < 2) {
		fprintf(stdout, "format: %s filename\n", argv[0]);
		return 1;
	}

	Tokenizer tokenizer;
	initTokenizer(&tokenizer, argv[1]);
	
	

	destroyTokenizer(&tokenizer);
	return 0;
}
