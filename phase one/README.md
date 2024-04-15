Introduction
 This document presents a C++ program designed to tokenize source code. Tokenization is the process of breaking down a sequence of characters into meaningful chunks called tokens. These tokens can include keywords, identifiers, numbers, strings, symbols, and whitespace. 

The program utilizes regular expressions to define patterns for each type of token in the C++ programming language. It then iterates through the input code, identifying and classifying tokens based on these patterns.

 Code Overview 
The C++ program consists of two main parts: the token structure definition and the tokenization function. 

Token Structure 
The `Token` structure is defined to store information about each token encountered during the tokenization process. It contains the following members
: - `type`: Represents the type of the token (e.g., Reservedword, Identifier, Number).
 - `lexeme`: Stores the actual text of the token.
 - `line`: Indicates the line number in the source code where the token was found. 

Tokenization Function (`tokenize`) The `tokenize` function takes a string `code` representing the source code as input and returns a vector of `Token` objects.

 Regular Expressions 
Several regular expressions are defined to match different types of tokens in C++:
 - `reservedword`: Matches reserved keywords in C++.
 - `identifier`: Matches valid identifiers.
 - `digit`: Matches single digits.
 - `number`: Matches numeric literals (integer or floating-point). 
- `stringsPattern`: Matches string literals enclosed in double quotes.
 - `symbol`: Matches various C++ symbols such as operators and punctuation. 
- `whitespacePattern`: Matches whitespace characters (spaces, tabs, newlines). 

These regular expressions are combined into one pattern (`tokenRegexStr`) for tokenization. 

# Tokenization Process

 The function iterates through the input `code` using regular expression search. For each match found, it creates a `Token` object and determines its type based on the matched pattern. Whitespace tokens are ignored. Line numbers are tracked to assign the correct line number to each token. Finally, the function returns the vector of tokens.

