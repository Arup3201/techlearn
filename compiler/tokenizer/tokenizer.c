#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>

#include "tokenizer.h"

typedef struct Tokenizer Tokenizer;
typedef enum TokenType TokenType;
typedef struct SymbolTable SymbolTable;

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

void retractForward(Tokenizer *t) {
	// if it is at the beginning of the active buffer 
	if(t->forward == t->buffers[t->active]) {
		t->active = 1 - t->active;
		t->forward = t->buffers[t->active];
	} else {
		t->forward--;
	}
}

void skipSpaces(Tokenizer *t) {
	while(*t->forward==' ' || *t->forward=='\t' || *t->forward=='\n') moveForward(t);
}

void skipComments(Tokenizer *t) {
	if(*t->forward == '-') {
		moveForward(t);

		if(*t->forward == '-') {
			// single line comment 
			while(*t->forward != SENTINEL && *t->forward != '\n') moveForward(t);

			// if we reached eof then return
			if(*t->forward == SENTINEL) return;
			
			// skip the new line 
			moveForward(t);
			return;
		} else {
			// not a single line comment retract forward
			retractForward(t);
			return;
		}
	}

	if(*t->forward == '/') {
		moveForward(t);

		if(*t->forward == '*') {
			// start of multi line comment 
			while(*t->forward != SENTINEL) {
				moveForward(t);
				
				if(*t->forward == '*') {
					moveForward(t);

					if(*t->forward == '/') {
						// end of multi line comment
						moveForward(t);
						return;
					}
				}
			}

			// we reached eof but multi line comment has not ended yet
		} else {
			// not a multi line comment retract forward 
			retractForward(t);
			return;
		}
	}

	return;
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
		skipSpaces(tokenizer);
		skipComments(tokenizer);

		// when the comment ended with eof instead of new line(single line comment) or */ (multi line comment)
		if(*tokenizer->forward == SENTINEL) {
			break;
		}

		if(
			(*tokenizer->forward >= 'a' && *tokenizer->forward <= 'z') 
			||
			(*tokenizer->forward >= 'A' && *tokenizer->forward <= 'Z')
			||
			(*tokenizer->forward == '_')
		  ) {
			
		}

		printf("%c", *tokenizer->forward);

		moveForward(tokenizer);
	}

	freeTokenizer(tokenizer);
	return 0;
}
