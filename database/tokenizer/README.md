# Tokenizer

A compiler has tokenizer to break the source code into small meaningful parts that can be processed. There are some terms like `token`, `lexeme` and `pattern`.

- `token`: It is the a category like keywords, identifiers etc which has some meaning and are the smallest unit of the source code.
- `pattern`: It is the structure that matches with the token.
- `lexeme`: It is the actual sequence of characters that matches a token's pattern.

In programming language, tokens are the smallest meaningful units that cannot be decomposed further. In languages like C, tokens can be `keywords`, `identifiers`, `operators` and `delimiters` (comma, semi-colon, braces etc.).

Tokens may be divided into 3 categories -

1. Terminal symbols - keywords and operators.
2. Literals - variables like numbers and strings.
3. Identifiers - user defined names.

Tokenizer is a rule-based program that performs lexical tokenization. Lexical tokenization is the conversion of a text into meaningful lexical tokens belonging to categories defined by the 'lexer' program. In case of natural language, those categories belong to nouns, adjectives, verbs etc. And in case of programming languages, those categories are identifiers, operators, grouping symbols and data types.

Lexical tokenization is based on lexical grammer. Lexing happens in 2 phases -

1. scanner - It segments the input string into syntactic units (lexemes), and then categorize them into token classes. It is usually based on finite state automata. It holds information regarding possible sequence of characters within a valid token that it can handle. It identifies the token class and assigns a type to the token.
2. evaluator - It converts the lexemes into processed values. A lexeme is a string of characters which we know is of a specific type. But to build a token, we also need the value. The lexeme's type with value is what a token is. Some lexemes do not need value like parenthesis. For those, evaluator can return nothing, only type is needed. Evaluators can also conceal some lexemes like comments and whitespaces.

For example, if the text is something like the following -

```
net_worth_future = (assets – liabilities);
```

Then after converting it into lexical token stream, whitespace is concealed and special characters has no value -

```
IDENTIFIER net_worth_future
EQUALS
OPEN_PARENTHESIS
IDENTIFIER assets
MINUS
IDENTIFIER liabilities
CLOSE_PARENTHESIS
SEMICOLON
```

In SQL the tokens can be divided into the following categories -

1. Keywords: `INSERT`, `DELETE`, `CREATE`, `UPDATE` etc.
2. Identifiers: `users`, `id`, `name` etc.
3. Operators: `>=`, `>`, `==` etc.
4. Literals: strings, integers, NULL etc.
5. Delimiters: `;`, `,`, `(`, `)` etc.
6. Comments: `--single line comment`, `/* multi-line comment */` etc.

For tokenizing, we need to first ignore the whitespace and comments. So, we need to find the whitespace and comments first.

1. Whitespace character: One of these five characters: u0009, u000a, u000c, u000d, or u0020.
```c
bool isSpace(char ch) {
	return (ch == 0x0009) || (ch == 0x000a) || (ch == 0x000d) || (ch == 0x0020);
}
```
2. Single line comments (`--`) and multi-line comments (`/*...*/`) should be ignored just like whitespace.
```c
bool isComment(const char** ptr) {
	// single line comments (--)
	if(*ptr[0] == 0x002d || *ptr[1] == 0x002d) {
		*ptr += 2;
		while(**ptr != '\n') (*ptr)++;
		return true;
	}

	// multi line comments (/* ... */)
	if(**ptr == 0x002f && *(*ptr+1) == 0x002a) {
		(*ptr) += 2;
		while(**ptr != 0x002a && *(*ptr+1) != 0x002f) (*ptr)++;
		(*ptr) += 1;
		return true;
	}

	return false;
}
```
