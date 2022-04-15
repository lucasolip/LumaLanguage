# Luma Language
Compiler for a personal programming language I call Luma, for a project from university subject _Language Processors_. Made using Flex, Bison and the C programming language.
The compiler/interpreter is written in C, and compiles Luma code into Q code. Q is a pseudo-assembly language based in C designed by my teacher.

## Compilation

     bison -dv analizadorsintactico.y
     flex analizadorlexico.l
     gcc -o analizador analizadorsintactico.tab.c lex.yy.c -lfl
