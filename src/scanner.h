#ifndef _SCANNER_H
#define _SCANNER_H

#include "strs.h"

typedef enum {
    ERROR = 200,
    BLOCK,          // '{' .*[^\n] '}'
    COLON,          // ':'
    SYMBOL,         // [a-zA-Z_][a-zA-Z0-9_]+
    STRG,           // '\"' .*[^\n] '\"'
    NUMBER,         // [0-9]+
    VERBOSITY,      // "%verbosity"
    AST_NAME,       // "%ast_name"
    PARSER_NAME,    // "%parser_name"
    AST_CODE,       // "%ast_code"
    PARSER_CODE,    // "%parser_code"
    AST_HEADER,     // "%ast_header"
    PARSER_HEADER,  // "%parser_header"
    END_OF_INPUT,   // file's end
} TokenType;

typedef struct {
    TokenType type;
    Str* str;
} Token;

TokenType get_token(Token* tok);
TokenType consume_token(Token* tok);
const char* tok_to_str(TokenType type);

#endif /* _SCANNER_H */
