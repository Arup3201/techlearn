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

- token: a token is a pair of token type and the token value. The token value is optional as some tokens do not have any value like keywords, operators etc. For identifiers, we need to store some information related to their name, data type and position in the sourc code for error mapping. So we keep a pointer to the symbol table where the identifier value is.
- lexeme: 
