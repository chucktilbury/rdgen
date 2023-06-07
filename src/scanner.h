#ifndef _SCANNER_H
#define _SCANNER_H

#include "strs.h"

typedef enum {
    ERROR = 200,
    BLOCK,        // '{' .*[^\n] '}'
    COLON,        // ':'
    SYMBOL,       // [a-zA-Z_][a-zA-Z0-9_]+
    STRG,         // '\"' .*[^\n] '\"'
    NUMBER,       // [0-9]+
    VERBOSITY,    // "%verbosity"
    PREFIX,       // "%prefix"
    NAME,         // "%name"
    CODE,         // "%code"
    END_OF_INPUT, // file's end
} TokenType;

typedef struct {
    TokenType type;
    STR str;
    int line;
    int col;
} Token;

TokenType get_token(Token* tok);
TokenType consume_token(Token* tok);
const char* tok_to_str(TokenType type);

#endif /* _SCANNER_H */
