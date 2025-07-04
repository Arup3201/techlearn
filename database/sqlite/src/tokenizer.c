#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdbool.h>
#include<stdlib.h>
#include<string.h>

#include "tokenizer.h"
#include "util.h"

typedef struct Tokenizer Tokenizer;
typedef enum TokenType TokenType;
typedef struct SymbolTable SymbolTable;

const int NUM_KEYWORDS = sizeof(KEYWORDS)/sizeof(KEYWORDS[0]);

Tokenizer* getTokenizer(char *fname) {
	Tokenizer *tokenizer = (Tokenizer*)malloc(sizeof(Tokenizer));

	tokenizer->tFd = open(fname, O_RDONLY, S_IRUSR);
	read(tokenizer->tFd, tokenizer->buffers[0], BUFFER_SIZE);
	tokenizer->buffers[0][BUFFER_SIZE] = SENTINEL;

	tokenizer->active = 0;
	tokenizer->lexemeBegin = tokenizer->buffers[tokenizer->active];
	tokenizer->forward = tokenizer->lexemeBegin;

	tokenizer->symbolTable = calloc(NUM_KEYWORDS, sizeof(SymbolTable));
	tokenizer->stLen = 0;

	for(int i=0; i<NUM_KEYWORDS; i++) {
		strcpy(tokenizer->symbolTable[i].lexeme, KEYWORDS[i]);
		tokenizer->symbolTable[i].token = TOK_KEYWORD;
		tokenizer->stLen += 1;
	}

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

bool isLetter_(char ch) {
	return ((ch >= 'a' && ch <= 'z') 
		||
		(ch >= 'A' && ch <= 'Z')
		||
		(ch == '_'));
}

char* getLexeme(Tokenizer *t) {
	printf("%p-%p\n", t->lexemeBegin, t->forward);
	char *lexeme;
	if(       ((t->lexemeBegin>=t->buffers[0] && t->lexemeBegin<t->buffers[0]+BUFFER_SIZE) 
		&& (t->forward >= t->buffers[1] && t->forward < t->buffers[1]+BUFFER_SIZE)) 
	   || 
		  ((t->lexemeBegin>=t->buffers[1] && t->lexemeBegin < t->buffers[1]+BUFFER_SIZE) 
		&& (t->forward >= t->buffers[0] && t->forward < t->buffers[0]+BUFFER_SIZE))
	  ) {
		int firstPartLen = t->buffers[1-t->active] + BUFFER_SIZE - t->lexemeBegin;
		int secondPartLen = t->forward - t->buffers[t->active];
		int length = firstPartLen+secondPartLen;
		lexeme = malloc(length+1);
		memcpy(lexeme, t->lexemeBegin, firstPartLen);
		memcpy(lexeme+firstPartLen, t->buffers[t->active], secondPartLen);
		lexeme[length] = '\0';
	} else {
		int length = t->forward - t->lexemeBegin + 1;
		lexeme = malloc(length+1);
		memcpy(lexeme, t->lexemeBegin, length);
		lexeme[length] = '\0';
	}

	return lexeme;
}

void installId(Tokenizer *t, char *lexeme) {
	for(int i=0; i<t->stLen; i++) {
		if(strcmp(t->symbolTable[i].lexeme, toLower(lexeme)) == 0) {
				return;
		}
	}

	strcpy(t->symbolTable[t->stLen].lexeme, lexeme);
	t->symbolTable[t->stLen].token = TOK_ID;
	t->stLen += 1;
}

TokenType getToken(Tokenizer *t, char *lexeme) {
	for(int i=0; i<t->stLen; i++) {
		if(strcmp(lexeme, t->symbolTable[i].lexeme) == 0) {
			return t->symbolTable[i].token;
		}
	}

	return TOK_ID;
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

		tokenizer->lexemeBegin = tokenizer->forward;

		if(isLetter_(*tokenizer->forward)) {
			while(isLetter_(*tokenizer->forward)) moveForward(tokenizer);
			retractForward(tokenizer);

			char *lexeme = getLexeme(tokenizer);
			// t contains the id/keyword
			installId(tokenizer, lexeme);

			// printf("%s -> %d\n", lexeme, getToken(tokenizer, lexeme));
		}

		moveForward(tokenizer);
	}

	freeTokenizer(tokenizer);
	return 0;
}
