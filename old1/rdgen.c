
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <ctype.h>

#include "ptrlst.h"
#include "strlst.h"
#include "rulelst.h"
#include "strs.h"
#include "memory.h"

/*
 * A rule has a name. The pattern is a list of pointers to a list of strings.
 * Each string in the list is a predicate of the rule. For example-
 *
 * rule_name
 *     : nonterm1 TERM nonterm2
 *     : nonterm3 TERMx nonterm4
 *
 * There will be a name and 2 predicates and each one will have 3 strings
 * in it.
 */
#define END_LINE    '\n'

typedef struct {
    Str* name;
    PtrLst* patterns;
} Rule;

typedef struct {
    FILE* fh;
    int ch;
    int line_no;
    int col_no;
    Str* ast_name;
    Str* parser_name;
    StrLst* ast_source;
    StrLst* ast_header;
    StrLst* parser_source;
    StrLst* parser_header;
    RuleLst* rules;
    bool finished;
    int errors;
} ParserState;

static inline int get_char(ParserState* state);
static inline int consume_space(ParserState* state);
static inline int consume_char(ParserState* state);

void syntax(ParserState* state, const char* fmt, ...) {

    va_list args;

    fprintf(stderr, "syntax error: %d: %d: ", state->line_no, state->col_no);

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fputc(END_LINE, stderr);
    state->errors++;
}

void consume_line(ParserState* state);

ParserState* create_parser(const char* fname) {

    ParserState* ptr = _alloc_obj(ParserState);

    ptr->fh = fopen(fname, "r");

    if(ptr->fh == NULL) {
        fprintf(stderr, "Cannot open input file: %s: %s\n", fname, strerror(errno));
        exit(1);
    }

    ptr->ch = 0;
    ptr->line_no = 1;
    ptr->col_no = 1;
    ptr->errors = 0;
    ptr->ast_source = create_lst();
    ptr->ast_header = create_lst();
    ptr->parser_source = create_lst();
    ptr->parser_header = create_lst();
    ptr->rules = create_lst();
    ptr->finished = false;

    ptr->ast_name = create_str(NULL);
    ptr->parser_name = create_str(NULL);

    // prime the stream of characters.
    ptr->ch = consume_char(ptr);

    return ptr;
}

int consume_comment(ParserState* state) {

    while(1) {
        state->ch = fgetc(state->fh);
        if(state->ch == END_LINE || state->ch == EOF) {
            state->line_no++;
            state->col_no = 1;
            state->ch = fgetc(state->fh);
            break;
        }
    }

    return state->ch;
}

static inline int get_char(ParserState* state) {

    return state->ch;
}

static inline int consume_space(ParserState* state) {

    int retv = 0;

    while(isspace(state->ch) && state->ch != EOF) {
        if(state->ch == END_LINE)
            retv++;
        consume_char(state);
    }

    return retv;
}

static inline int consume_char(ParserState* state) {

    state->ch = fgetc(state->fh);

    if(state->ch != EOF) {
        if(state->ch == END_LINE) {
            state->line_no++;
            state->col_no = 1;
        }
        else if(state->ch == '#') {
            consume_comment(state);
        }
        else
            state->col_no++;
    }
    else
        state->finished = true;

    return state->ch;
}

bool is_finished(ParserState* state) {

    return state->finished;
}

// When this is entered, the directive has been figured out and the correct
// string buffer is in the name parameter.
void get_name(ParserState* state, Str* name) {

    consume_space(state);
    int ch = get_char(state);

    if(ch != '\"') {
        syntax(state, "expected quoted string but got %c", ch);
        exit(1);
    }
    consume_char(state); // consume the '\"'

    for(ch = get_char(state); ch != '\"'; ch = consume_char(state)) {
        if(ch == EOF) {
            syntax(state, "unexpected end of file");
            syntax(state, "unterminated quoted string");
            exit(1);
        }

        cat_str_char(name, ch);
    }
}

int get_symbol(ParserState* state, Str* name) {

    consume_space(state);
    int ch = get_char(state);

    if(isalpha(ch) || ch == '_') {
        cat_str_char(name, ch);
        ch = consume_char(state);
    }

    while(isalpha(ch) || ch == '_') {
        cat_str_char(name, ch);
        ch = consume_char(state);
    }

    return name->len;
}

void get_code_block(ParserState* state, StrLst* lst) {

    consume_space(state);
    int ch = get_char(state);
    int count = 0;
    Str* tmp = create_str(NULL);

    if(ch != '{') {
        syntax(state, "expected a '{' but got %c", ch);
        exit(1);
    }
    cat_str_char(tmp, ch);
    consume_char(state); // consume the '{'
    count++;

    while(count > 0) {
        ch = get_char(state);
        if(ch == EOF) {
            syntax(state, "unexpected end of file");
            syntax(state, "unterminated code block. Expected a '}'");
            exit(1);
        }
        else if(ch == '{')
            count++;
        else if(ch == '}')
            count--;

        // don't care what the string contains.
        cat_str_char(tmp, ch);
        consume_char(state);
    }

    add_str_lst(lst, tmp);
}

void get_directive(ParserState* state) {

    Str* str = create_str(NULL);
    int count = 0;
    StrLst* lst = NULL;

    while(true) {
        int ch = get_char(state);

        if(ch == EOF) {
            syntax(state, "invalid directive: %s", str->buffer);
            syntax(state, "unexpected end of file");
            exit(1);
        }
        else if(isspace(ch))
            break;
        else if(ch == '{') {
            count++;
            break;
        }
        else
            cat_str_char(str, ch);

        consume_char(state);
    }

    // At this point, we have the name, including the '%' at the front.
    char* tmp = str->buffer;
    if(strcmp(tmp, "%psource") == 0) {
        lst = state->parser_source;
    }
    else if(strcmp(tmp, "%asource") == 0) {
        lst = state->ast_source;
    }
    else if(strcmp(tmp, "%pheader") == 0) {
        lst = state->parser_header;
    }
    else if(strcmp(tmp, "%aheader") == 0) {
        lst = state->ast_header;
    }
    else if(strcmp(tmp, "%aname") == 0) {
        get_name(state, state->ast_name);
        return;
    }
    else if(strcmp(tmp, "%pname") == 0) {
        get_name(state, state->parser_name);
        return;
    }
    else {
        syntax(state, "invalid directive: \"%s\"", tmp);
        return;
    }

    if(lst == NULL) {
        syntax(state, "error reading directive");
        return;
    }

    get_code_block(state, lst);
}

int get_rule_pred(ParserState* state, Rule* rule) {

    consume_space(state);
    int retv = 0;


    while(true) {
        if(consume_space(state) > 0)
            return 0;

        Str* tmp = create_str(NULL);
        if(!get_symbol(state, tmp))
            break;

        add_lst(rule->patterns, tmp);
        retv++;
    }

    return retv;
}

int get_rule_pred_lst(ParserState* state, Rule* rule) {

    int retv = 0;
    int ch = get_char(state);

    // next character must be a ':'
    while(true) {
        if(ch != ':')
            return 0;
        consume_char(state);

        if(get_rule_pred(state, rule) > 0)
            retv++;
        else
            break;
    }

    return retv;
}

Rule* create_rule(Str* name) {

    Rule* rule = _alloc_obj(Rule);
    rule->name = name;
    rule->patterns = create_lst();

    return rule;
}

void get_rule(ParserState* state) {

    consume_space(state);
    //int ch = get_char(state);
    Str* name = create_str(NULL);
    Rule* rule = NULL;

    if(get_symbol(state, name) > 0) {
        rule = create_rule(name);
        if(!(get_rule_pred_lst(state, rule) > 0)) {
            syntax(state, "no rule predicates found");
            return;
        }
    }

    add_lst(state->rules, rule);
}

void dump_state(ParserState* state) {

    printf("ast_name: \"%s\"\n", state->ast_name->buffer);
    printf("parser_name: \"%s\"\n", state->parser_name->buffer);
}

int main(int argc, char** argv) {

    if(argc < 2) {
        fprintf(stderr, "ERROR: file name is required.\n");
        return 1;
    }

    ParserState* state = create_parser(argv[1]);

    while(!is_finished(state)) {
        consume_space(state);
        int ch = get_char(state);
        if(ch == '%')
            get_directive(state);
        else
            get_rule(state);
    }

    dump_state(state);
    return 0;
}
