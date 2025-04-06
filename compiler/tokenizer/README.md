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
