# Tokenizer

In a compiler, a tokenizer is part of the lexical analysis phase. Tokenizer generates tokens that is parsed by the parser. Tokens are pair of lexemes and values. Some tokens do not have values like keywords, operators. But some tokens needs value like identifiers.

We will use lexical analyzer term to denote tokenizer. Because in the compiler that is the term that is used. 

Lexical analyzer is mainly responsible for reading the input characters, converting them into lexemes and produce a sequence of tokens for each lexeme in the source program. The stream of tokens goes to parser for syntax analysis. It is common for lexical analyzer to interact with the symbol table. When the lexical analyzer encounters any identifier it needs to store that identifier in the symbol table.

The interaction between lexical analyzer, symbol table and parser happens as follows -

- Parser calls the lexical analyzer for the next token in the input 
- Lexical analyzer gets the next token, it also uses symbol table if there is any identifier 
- After the token is produced, it goes to parser 

This happens untill lexical analyzer can detect token in the input sequence.

Because lexical analyzer is the part that reads the source text, it is also responsible for stripping the whitespace, tabs, comments and other characters that are used to seperate tokens in the input. It may also be responsible for correlating error messages with the source program.

Lexical analysis is divided into 2 tasks 

- Scanning: Lexer reads the input, delete the comments and compacts the whitespaces into one whitespace.
- Lexical analysis: Lexer will read the output of the scanner, then produce the tokens from the output of the scanner.

In lexical analysis, following terms will be used a lot -

- token: a token is a pair consisting of the token name and the token value. The token value is optional as some tokens do not have any value like keywords, operators etc. For identifiers, we need to store some information related to their name, data type and position in the sourc code for error mapping. For that, we keep a pointer to the symbol table where the identifier value is.
- pattern: a pattern is the description of the form of the token. For keyword the lexeme is just the sequence of the keyword, for identifiers it will be more complex form as it is matched with many string formats.
- lexeme: a lexeme is a sequence of characters in the source code that matches the pattern of a token, and is identified by the analyzer as an instance of a token.

```c
printf("Total = %d\n", score);
```

In this C statement, `printf` and `score` are the lexemes that match the pattern of **id** token, and `Total = %d\n"` is the lexeme that match the pattern of a **literal** token.

When implementing lexical analyzer, we need to take care of how we read the input source code so that we can classify them into tokens. To be sure that a lexeme follows the pattern of a token, we need to have a large lookahead safety.

If we have the following source code -

```f90
E = M*C**2
```

The tokens will be -

```
<id, pointer to symbol table value E>
<eq_op>
<id, pointer to symbol table value M>
<mult_op>
<id, pointer to symbol table value C>
<exp_op>
<number, integer value 2>
```

## Input Buffer

We are going to use two-buffer scheme that handles large lookaheads safely. Because source code could be really long, we use 2 buffers that have the same size N, N is usually the size of block in a disk i.e. 4096 bytes. When reading the source code, the buffers are alternatively reloaded.

When we read the source code, we read the N characters using a system read call. If the source code has less than N characters then **eof** is used to mark the end of input.


The two-buffer scheme works as follows -

```
BUFFER 1   | BUFFER 2
E| |=| |M|*|C|*|*|2|eof
              ^   ^
     lexemBegin   forward
```

We keep track of 2 pointers -

- **lexemeBegin** points to the beginning of the current lexeme whose token type we are trying to find out 
- **forward** scans ahead untill an exact match is found

Once the lexeme's token name is determined, we move the **forward** to the right end of the current lexeme, send the attribute value to parser (optional), and then move **lexemeBegin** to the beginning of the next lexeme.

When moving the **forward** we need to check if we have reached the end of the current buffer? If so then we need to reload the other buffer and set **forward** at the beginnig of the other buffer.

When moving the **forward** we need to check 2 things - whether we have reached end of the buffer and what character did we read. We can combine these 2 checks into 1 by using **sentinels** at the end of each buffer. Here, we will use **eof** as the sentinel because it is a special character that will never appear in the source code.

**eof** can appear when it is the end of input string or it is the end of buffer.

The way of reading the source code with 2 buffers is shown here:

```c
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
```

Untill the `forward` is pointing to sentinel, we will process the source code character by character and every time we move the `forward` pointer with the following logic:

```c
void moveForward(Tokenizer *t) {
	if(*(t->forward+1) == SENTINEL) {
		reloadBuffer(t);
		t->forward = t->buffers[t->active];
	} else {
		t->forward++;
	}
}
```

When the next character is sentinel, we reload the second buffer (assuming the first buffer was active) and then `forward` will point to the beginning of the second buffer, Otherwise we just increase the `forward` by 1.

In this code, I have used `Tokenizer` which has the following structure:

```c
struct Tokenizer {
	int tFd; // file descriptor of the file we are tokenizing
	char buffers[2][BUFFER_SIZE+1]; // 2 buffer scheme 
	int active; // active buffer 
	char *lexemeBegin;
	char *forward;
};
```

We first get the tokenizer and at last we destroy the tokenizer. When initializing the tokenizer object, we read the source code, and by default make the first buffer active and put the source code (partial) in that buffer.

```c
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
```

When reloading the buffer, we set the first character of the previously active buffer equal to sentinel so that the stopping condition in the `main` function work.

```c
void reloadBuffer(Tokenizer *t) {
	*t->buffers[t->active] = SENTINEL;

	t->active = 1 - t->active;
	read(t->tFd, t->buffers[t->active], BUFFER_SIZE);
}
```

## Regular Expression

In lexical analysis the most important operations on languages are union, concatenation and closures. Union is the same operation as in sets (e.g. {1, 2}U{3, 4} = {1, 2, 3, 4}). Concatenation is the operation of taking a string from first language and another string from second language, in all possible ways and concatenating them together. Closure can be Kleene closure and positive closure. In Kleene closure, denoted as $L^*$, we concatenate strings from L zero or more times. In positive closure, denoted by $L^+$ we concatenate strings from L one or more times.

Some extensions of regular expressions:

- one or many instances: (r+)
- zero or one instance: (r?)
- character class: when the regular expression makes a logical sequence, it can be written with character class e.g. a|b|c|...|z can be written as [a-z]. For digits it can be [0-9] etc.

In C language identifiers can be written with the following regular expression:

```
letter_ = [A-Za-z_]
digit = [0-9]
id = letter_ | (letter_ | digit)
```

Numbers can be represented with the following regular expression:

```
digit = [0-9]
digits = digit+
number = digits (.digits)? (E[+-]?digits)?
```

## Recognition of Tokens 

We give the job of splitting the source code using whitespace `ws`. `ws` has the following regular expression:

```
ws = (tab | blank | new line)+
```

We also need to avoid comments (single and multi line comments) and then try to check which token type the lexeme is.

Following code checks whether the source code contains `ws` or not, if so then it will move the `forward` pointer till non `ws` character in the source code.

```c
void skipSpaces(Tokenizer *t) {
	while(*t->forward==' ' || *t->forward=='\t' || *t->forward=='\n') moveForward(t);
}
```

It is a little complex for comments. For single line comment and multi line comment we check them with the following code:

```c
void skipComments(Tokenizer *t) {
	if(*t->forward == '-') {
		moveForward(t);

		if(*t->forward == '-') {
			while(*t->forward != SENTINEL && *t->forward != '\n') moveForward(t);

			if(*t->forward == SENTINEL) return;
			
			moveForward(t);
			return;
		} else {
			retractForward(t);
			return;
		}
	}

	if(*t->forward == '/') {
		moveForward(t);

		if(*t->forward == '*') {
			while(*t->forward != SENTINEL) {
				moveForward(t);
				
				if(*t->forward == '*') {
					moveForward(t);

					if(*t->forward == '/') {
						moveForward(t);
						return;
					}
				}
			}

		} else {
			retractForward(t);
			return;
		}
	}

	return;
}
```

Let's break down the `skipComments` function:

- if we encounter `-` then it might be the start of the single line comment. So, we move forward for the next character. If the next character is another `-` then we know it is a single line comment otherwise we retract forward by 1 using `retractForward` function.
- if it is a single line comment then we move forward untill we reach a new line or the end of the file.
- if we reach end of file in the case of single line comment then we return here otherwise we move forward by one to the new line character.
- if we encounter `/` then it might be the start of a multi-line comment, we move forward by 1 to check if the next character is `*`. If it is, then we know it is the start of a multi line comment otherwise we retract the `forward` by 1 and return.
- if it is the start of a multi line comment then we move forward untill we reach the end of file or `*`.
- if we reach end of file, then it means the multi line comment did not end
- if we reach `*` then we move forward by 1 and check the next character is `/` or not. If it is then we are at the end of multi line comment. So we move forward by 1 and return.
- if we do not find `/` then we proceed with the while loop.

The loop will updated with the `skipSpaces` and `skipComments` as follows:

```c
while(*tokenizer->forward != SENTINEL) {
    skipSpaces(tokenizer);
    skipComments(tokenizer);

    if(*tokenizer->forward == SENTINEL) {
        break;
    }

    moveForward(tokenizer);
}
```

Because the `skipComments` function can proceed the `forward` till eof, we need to put a check on that and break the loop if true.

The code to retract the `forward` is as follows:

```c
void retractForward(Tokenizer *t) {
	if(t->forward == t->buffers[t->active]) {
		t->active = 1 - t->active;
		t->forward = t->buffers[t->active];
	} else {
		t->forward--;
	}
}
```

### Seperating id and reserved keywords

Keywords also follow the same pattern like id. So, it is important to seperate them. To seperate we do the following:

- we create a symbol table which contains token type, and attribute value.
- when we encounter an id we call `installId` to install the identifier in the symbol table if that identifier does not exist, then we return the token type which is id.
- if the id is a keyword then the `installId` call will not do anything as there is already a token with that name and `getToken` returns keywords instead of id.


