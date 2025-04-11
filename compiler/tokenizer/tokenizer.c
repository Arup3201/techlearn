#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>

#include "tokenizer.h"

typedef struct Tokenizer Tokenizer;
typedef enum TokenType TokenType;

Tokenizer* getTokenizer(char *fname) {
	Tokenizer *tokenizer = (Tokenizer*)malloc(sizeof(Tokenizer));

	tokenizer->tFd = open(fname, O_RDONLY, S_IRUSR);
	read(tokenizer->tFd, tokenizer->buffers[0], BUFFER_SIZE);
	tokenizer->buffers[0][BUFFER_SIZE] = SENTINEL;

	tokenizer->active = 0;
	tokenizer->lexemeBegin = tokenizer->buffers[tokenizer->active];
	tokenizer->forward = tokenizer->lexemeBegin;

	return tokenizer;
}

void reloadBuffer(Tokenizer *t) {
	*t->buffers[t->active] = SENTINEL;

	t->active = 1 - t->active;
	read(t->tFd, t->buffers[t->active], BUFFER_SIZE);
}

void moveForward(Tokenizer *t) {
	if(*(t->forward+1) == SENTINEL) {
		reloadBuffer(t);
		t->forward = t->buffers[t->active];
	} else {
		t->forward++;
	}
}

bool isSpace(char ch) {
	return ch == ' ';
}

void freeTokenizer(Tokenizer *t) {
	close(t->tFd);
	t->lexemeBegin = NULL;
	t->forward = NULL;
	free(t);
}

int main(int argc, char* argv[]) {
	if(argc < 2) {
		fprintf(stdout, "format: %s filename\n", argv[0]);
		return 1;
	}

	Tokenizer *tokenizer;
	tokenizer = getTokenizer(argv[1]);	

	while(*tokenizer->forward != SENTINEL) {
		printf("%c", *tokenizer->forward);

		moveForward(tokenizer);
	}

	freeTokenizer(tokenizer);
	return 0;
}
