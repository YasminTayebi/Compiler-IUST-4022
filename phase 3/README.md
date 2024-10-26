## Overview
This project implements a basic C++ compiler capable of analyzing and generating intermediate code for a subset of C++ programs. It demonstrates fundamental concepts of lexical analysis, syntax parsing, and intermediate code generation.

## Features
- **Tokenization**: The compiler breaks down C++ source code into tokens such as reserved words (`int`, `while`), identifiers (variable names), numbers, symbols (operators), and strings.
  
- **Parsing**: It verifies the syntax of the code and constructs a parse tree. This process ensures that the structure of the code follows the rules of the subset of C++ supported by the compiler.

- **Intermediate Code Generation**: The compiler generates Three Address Code (TAC) as an intermediate representation of the source code. TAC is a sequence of simple instructions that can be easily translated into machine code.

- **File Output**: The generated TAC is saved to a file (`output.txt`), providing a record of the translated code.

## Usage
To use the compiler:
1. **Input**: Define your C++ source code as a string in the `code` variable within `main()`.
2. **Compilation**: Compile and run the program. It will tokenize the code, parse it, and generate the intermediate code.
3. **Output**: Check the `output.txt` file to view the generated Three Address Code.

## Purpose
This project serves as an educational tool to understand the basics of compiler construction:
- **Lexical Analysis**: Breaking down code into meaningful tokens.
- **Syntax Parsing**: Ensuring code adheres to the defined grammar rules.
- **Intermediate Code Generation**: Creating an intermediate representation for further processing.

It emphasizes the importance of structured analysis in software development, ensuring programs are correct, efficient, and maintainable.

