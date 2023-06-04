# RDPGEN
Recursive decent parser generator.

This is a recursive parser generator for the C language. The reason for this is that I want to write a RD parser for my language called Simple, however, writing a parser generator is just as much work as generating the actual parser, so it makes sense to do it this way. I want a parser generator that has a very simple input syntax and will simply generate a RD parser and a AST to associate to it. It is outputting C because The actual compiler for Simple will be written in C or C++.

The idea is to write a translator that accepts a language similar to Bison, but much more simple, and output the C code to create an AST based upon the grammar. There are no basic restrictions on the grammar. The grammar is not trying to be LALR or GLR or whatever. The grammar is also not PEG. The translator simply takes the rules that are presented in the input and creates the functions to implement a RD parser, an AST, and the methods to traverse the AST.

The code that is needed to debug the grammar will be built into the C output. This basically is a simple way to trace through the rules that are being used to parse a construct.

The code that it output by RDPGEN is intentionally pedantic and simple. There are no "fancy" tricks used, such as goto or setjmp. All simple and clear C.

Provisions are made to put arbitrary code into the parser in various ways, such as at the beginning or the end of the C output or in the headers. Any arbitrary C code can be added to rules, but the code is added to the AST visitor instead of the parser.

Provisions for including files is handled in the scanner, which is not implemented by this functionality. The code is built around using something like Flex or a hand-written scanner.

The translation happens in several passes and a lot of functions are created.
- A list of rule names is created and the rule content is added to them.
- The rules content is scanned and duplicate items are added to the function list. The list is repeatedly scanned until there are no more duplicates found.
- The connections between the functions are created and stored in a data structure.
- Error conditions are identified and stored in the data structure. (i.e. expected x but got y)
- The C code is emitted in 2 modules, the parser and the AST.

## Build package dependencies
- graphiz-dev
- libgc-dev
