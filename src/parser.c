#include <stdbool.h>
#include <stdlib.h>

#include "parser.h"
#include "scanner.h"
#include "fileio.h"
#include "memory.h"
#include "errors.h"

PTRLST_IMPL(str_lst, Str*)
PTRLST_IMPL(rule_lst, Rule*)
PTRLST_IMPL(pattern_lst, Pattern*)

static Pstate* create_parser(const char* fname) {

    Pstate* state = _alloc_obj(Pstate);

    read_file(fname);
    consume_token(NULL); // prime the pipeline.

    state->finished      = false;
    state->verbo         = 5;
    state->ast_source    = str_lst_create();
    state->ast_header    = str_lst_create();
    state->parser_source = str_lst_create();
    state->parser_header = str_lst_create();
    state->terminals     = str_lst_create();
    state->non_terminals = str_lst_create();
    state->rules         = rule_lst_create();
    state->ast_name      = create_str(NULL);
    state->parser_name   = create_str(NULL);

    return state;
}

static Pattern* parse_pattern() {

    Token tok;
    Pattern* pat = _alloc_obj(Pattern);
    pat->lst = str_lst_create();

    consume_token(&tok);

    while(tok.type == SYMBOL) {
        str_lst_add(pat->lst, create_str(tok.str->buffer));
        consume_token(&tok);
    }

    if(get_token(&tok) == BLOCK)
        pat->code = create_str(tok.str->buffer);
    else
        syntax("expected a code block at the end of the pattern, but got %s", tok_to_str(tok.type));

    return pat;
}

static void parse_rule(Pstate* state) {

    Token tok;
    get_token(&tok); // consume the directive name

    if(tok.type == SYMBOL) {
        Rule* rule = _alloc_obj(Rule);
        rule->patterns = pattern_lst_create();

        get_token(&tok);
        rule->name = create_str(tok.str->buffer);

        while(consume_token(&tok) == COLON) {
            pattern_lst_add(rule->patterns, parse_pattern());
        }
        if(rule->patterns->len == 0)
            syntax("expected a rule pattern but got a %s", tok_to_str(get_token(NULL)));

        rule_lst_add(state->rules, rule);
    }
    else
        syntax("expected a symbol to start a rule but got a %s", tok_to_str(tok.type));

    //consume_token(NULL); // consume the number
}

static void parse_verbo(Pstate* state) {

    TokenType type = consume_token(NULL); // consume the directive name
    if(type == NUMBER) {
        Token tok;
        get_token(&tok);
        state->verbo = (int)strtol(tok.str->buffer, NULL, 10);
    }
    else
        syntax("expected a number but got a %s", tok_to_str(type));

    consume_token(NULL); // consume the number
}

static void parse_ast_name(Pstate* state) {

    TokenType type = consume_token(NULL); // consume the directive name
    if(type == STRG) {
        Token tok;
        get_token(&tok);
        state->ast_name = create_str(tok.str->buffer);
    }
    else
        syntax("expected a string but got a %s", tok_to_str(type));

    consume_token(NULL); // consume the string
}

static void parse_parser_name(Pstate* state) {

    TokenType type = consume_token(NULL); // consume the directive name
    if(type == STRG) {
        Token tok;
        get_token(&tok);
        state->parser_name = create_str(tok.str->buffer);
    }
    else
        syntax("expected a string but got a %s", tok_to_str(type));

    consume_token(NULL); // consume the string
}

static void parse_ast_code(Pstate* state) {

    TokenType type = consume_token(NULL); // consume the directive name
    if(type == BLOCK) {
        Token tok;
        get_token(&tok);
        str_lst_add(state->ast_source, create_str(tok.str->buffer));
    }
    else
        syntax("expected a code block but got a %s", tok_to_str(type));

    consume_token(NULL); // consume the code block
}

static void parse_parser_code(Pstate* state) {

    TokenType type = consume_token(NULL); // consume the directive name
    if(type == BLOCK) {
        Token tok;
        get_token(&tok);
        str_lst_add(state->parser_source, create_str(tok.str->buffer));
    }
    else
        syntax("expected a code block but got a %s", tok_to_str(type));

    consume_token(NULL); // consume the code block
}

static void parse_ast_header(Pstate* state) {

    TokenType type = consume_token(NULL); // consume the directive name
    if(type == BLOCK) {
        Token tok;
        get_token(&tok);
        str_lst_add(state->ast_header, create_str(tok.str->buffer));
    }
    else
        syntax("expected a code block but got a %s", tok_to_str(type));

    consume_token(NULL); // consume the code block
}

static void parse_parser_header(Pstate* state) {

    TokenType type = consume_token(NULL); // consume the directive name
    if(type == BLOCK) {
        Token tok;
        get_token(&tok);
        str_lst_add(state->parser_header, create_str(tok.str->buffer));
    }
    else
        syntax("expected a code block but got a %s", tok_to_str(type));

    consume_token(NULL); // consume the code block
}

Pstate* parse_input(const char* fname) {

    Pstate* state = create_parser(fname);
    bool finished = false;

    while(!finished && get_errors() == 0) {
        TokenType type = get_token(NULL);
        switch(type) {
            case SYMBOL:        parse_rule(state);          break;
            case VERBOSITY:     parse_verbo(state);         break;
            case AST_NAME:      parse_ast_name(state);      break;
            case PARSER_NAME:   parse_parser_name(state);   break;
            case AST_CODE:      parse_ast_code(state);      break;
            case PARSER_CODE:   parse_parser_code(state);   break;
            case AST_HEADER:    parse_ast_header(state);    break;
            case PARSER_HEADER: parse_parser_header(state); break;
            case END_OF_INPUT:  finished = true;            break;
            default:
                syntax("expected rule or directive but got %s", tok_to_str(type));
       }
    }

    return state;
}
