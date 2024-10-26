Project Overview

This project implements a fundamental C++ compiler designed to analyze and comprehend a simplified subset of C++ code. It primarily focuses on two crucial aspects of compiler design: lexical analysis and syntax analysis.
Features

    Tokenization: The compiler processes input C++ code to extract meaningful units known as tokens. These tokens encompass reserved words (e.g., int, while), identifiers (variable names), numbers, strings, and symbols (such as arithmetic operators).

    Parsing: After tokenization, the compiler checks the code's syntax. It constructs a hierarchical structure, known as a parse tree, which illustrates the syntactic relationships among various code elements. This process helps in understanding how statements are organized within functions and loops.

    Parse Tree Visualization: The generated parse tree is printed to provide a clear visual representation of the code’s structure. This visualization aids in grasping the organization and flow of statements and expressions within the source code.

Usage

To utilize the compiler:

    Provide your C++ source code as a string input.
    The compiler will tokenize the string, categorizing each part into distinct tokens.
    It will then verify the arrangement of these tokens against established syntax rules to ensure correctness.
    Finally, the compiler generates and prints a parse tree, offering a visual representation of how different code components are interconnected.

Purpose

This project serves as an educational resource to explore the essential processes involved in translating human-readable code into a machine-executable format. It highlights the significance of structured analysis in creating programs that are not only correct but also efficient and maintainable.