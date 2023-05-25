#include <stdio.h>

#include "dump.h"
#include "emit.h"
#include "errors.h"
#include "memory.h"
#include "parser.h"
#include "preproc.h"
#include "rdpgen.h"

/*
 * Main entry point.
 */
int main(int argc, char** argv) {

    Pstate* state = parse_input(argc > 1? argv[1]: NULL);

    if(get_errors() == 0)
        pre_process(state);

    if(get_errors() == 0) {
        emit(state);
        //dump_state(state);
    }

    fprintf(stdout, "errors: %d\n", get_errors());

    return 0;
}

#if 0
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
#define END_LINE '\n'

PTRLST_IMPL(str_lst, Str*)
PTRLST_IMPL(rule_lst, Rule*)
PTRLST_IMPL(pattern_lst, Pattern*)

static inline int consume_comment(Pstate* state) {

    while(true) {
        state->ch = fgetc(state->fh);
        if(state->ch == END_LINE || state->ch == EOF) {
            state->line_no++;
            state->col_no = 1;
            state->ch     = fgetc(state->fh);
            break;
        }
    }

    return state->ch;
}

static inline int get_char(Pstate* state) {

    return state->ch;
}

// only used with get_block().
static inline int consume_raw_char(Pstate* state) {

    state->ch = fgetc(state->fh);

    if(state->ch != EOF) {
        if(state->ch == END_LINE) {
            state->line_no++;
            state->col_no = 1;
        }
        else
            state->col_no++;
    }
    else
        state->finished = true;

    return state->ch;
}

static inline int consume_char(Pstate* state) {

    state->ch = fgetc(state->fh);

    if(state->ch != EOF) {
        /*if(state->ch == '#') {
            consume_comment(state);
        }
        else */ if(state->ch == END_LINE) {
            state->line_no++;
            state->col_no = 1;
        }
        else {
            state->col_no++;
        }
    }
    else {
        state->finished = true;
    }

    return state->ch;
}

static inline int consume_space(Pstate* state) {

    int retv = 0;

    while((isspace(state->ch) && state->ch != EOF) || state->ch == '#') {
        if(state->ch == END_LINE)
            retv++;
        else if(state->ch == '#')
            consume_comment(state);

        consume_char(state);
    }

    return retv;
}

static inline bool is_finished(Pstate* state) {

    return state->finished;
}

#define CHAR get_char(state)
#define CONSUME consume_char(state)
#define SPACE consume_space(state)
#define FINISHED is_finished(state)

/*
 * Get a quoted string from the input stream. No checking is done
 * for the contents. String is enclosed with '\"' characters.
 */
Str* get_qstr(Pstate* state) {

    SPACE;
    int ch   = CHAR;
    Str* str = NULL;

    if(ch != '\"') {
        SYNTAX("expected quoted string but got %c", ch);
        return NULL;
    }
    else {
        str = create_str(NULL);
        CONSUME; // consume the '\"'
        for(ch = CHAR; ch != '\"'; ch = CONSUME) {
            if(ch == EOF) {
                SYNTAX("unexpected end of file");
                SYNTAX("unterminated quoted string");
                return NULL;
            }

            cat_str_char(str, ch);
        }
        // if the string is empty, return an empty string buffer.
    }

    CONSUME;
    return str;
}

/*
 * Retrieve a string that does not have spaces, begins with isalpha() and
 * has only isalnum() in it. Also a underline is acceptable.
 */
Str* get_name(Pstate* state) {

    SPACE;
    int ch   = CHAR;
    Str* str = NULL;

    if(!(isalpha(ch) || ch == '_')) {
        SYNTAX("expected a valid name (%c 0x%02X)", ch, ch);
        return NULL;
    }
    else {
        str = create_str(NULL);
        cat_str_char(str, ch);
        CONSUME;
        while(true) {
            ch = CHAR;
            if(isalnum(ch) || ch == '_')
                cat_str_char(str, ch);
            else
                break;
            CONSUME;
        }
    }

    return str;
}

/*
 * A code block starts with a '{' and ends with a '}'. They are included in
 * it and the block is read as a single string. The brackets in the string must
 * be balanced.
 */
Str* get_block(Pstate* state) {

    SPACE;
    int ch    = CHAR;
    Str* str  = NULL;
    int count = 0;

    if(ch != '{') {
        SYNTAX("expected a code block enclosed with {}");
        return NULL;
    }
    else {
        consume_raw_char(state);
        count++;
        str = create_str(NULL);
        cat_str_char(str, ch);
        while(count > 0) {
            ch = CHAR;
            if(ch == EOF) {
                SYNTAX("unexpected end of file");
                SYNTAX("missing '}'");
                return NULL;
            }
            else if(ch == '{')
                count++;
            else if(ch == '}')
                count--;

            cat_str_char(str, ch);
            consume_raw_char(state);
        }
    }

    return str;
}

/*
 * Get a directive. This is indicated by finding a '%' as the first character
 * in a line. There are block directives and name directives. The name
 * directives accept a quoted string and the block directives accept an
 * arbitrary block of C code that will be run inside a function.
 *
 * When this is entered a '%' has been read and it the current character in
 * the character stream. The next character after the '%' must be the
 * directive that is to be parsed.
 */
void get_directive(Pstate* state) {

    Str* name = create_str(NULL);
    cat_str_char(name, CHAR);
    CONSUME;

    // read the directive name
    while(true) {
        int ch = CHAR;
        if(!isalnum(ch) || isspace(ch) || ch == EOF) {
            break;
        }
        else
            cat_str_char(name, ch);

        CONSUME;
    }

    if(strcmp(name->buffer, "%psource") == 0)
        str_lst_add(state->parser_source, get_block(state));
    else if(strcmp(name->buffer, "%asource") == 0)
        str_lst_add(state->ast_source, get_block(state));
    else if(strcmp(name->buffer, "%pheader") == 0)
        str_lst_add(state->parser_header, get_block(state));
    else if(strcmp(name->buffer, "%aheader") == 0)
        str_lst_add(state->ast_header, get_block(state));
    else if(strcmp(name->buffer, "%pname") == 0)
        state->parser_name = get_qstr(state);
    else if(strcmp(name->buffer, "%aname") == 0)
        state->ast_name = get_qstr(state);
    else
        SYNTAX("invalid directive: %s", name->buffer);
}

/*
 * Allocate memory for a new rule.
 */
Rule* create_rule() {

    Rule* rule     = _alloc_obj(Rule);
    rule->name     = NULL;
    rule->patterns = pattern_lst_create();

    return rule;
}

/*
 * When this is entered, a pattern has been seen. The ':' has already been
 * consumed. This is a simple list of words. All of the words must be on a
 * single line.
 */
Pattern* get_pattern(Pstate* state) {

    Pattern* pat = _alloc_obj(Pattern);
    pat->lst     = str_lst_create();

    while(SPACE == 0) {
        str_lst_add(pat->lst, get_name(state));
    }

    return pat;
}

/*
 * When this is entered, a rule name has been read. The next thing is a list
 * of lines where each one starts with a ':'.
 */
void get_pattern_lst(Pstate* state, Rule* rule) {

    SPACE;
    while(true) {
        if(CHAR != ':') {
            break;
        }
        else {
            CONSUME; // consume the ':'
            pattern_lst_add(rule->patterns, get_pattern(state));
        }
    }
}

/*
 * When this is entered, the object being parsed is not a directive, so it
 * must be a rule. The current character is the first character of a
 * rule name.
 */
void get_rule(Pstate* state) {

    Rule* rule = create_rule();

    SPACE;
    rule->name = get_name(state);
    if(rule->name == NULL)
        SYNTAX("cannot read a rule name");
    else {
        get_pattern_lst(state, rule);
        rule_lst_add(state->rules, rule);
    }
}

Pstate* create_parser(const char* fname) {

    Pstate* state = _alloc_obj(Pstate);

    state->fh = fopen(fname, "r");

    if(state->fh == NULL) {
        fprintf(stderr, "Cannot open input file: %s: %s\n", fname, strerror(errno));
        exit(1);
    }

    state->finished      = false;
    state->ch            = 0;
    state->line_no       = 1;
    state->col_no        = 1;
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

    // prime the stream of characters.
    state->ch = consume_char(state);

    while(!FINISHED) {
        SPACE;
        int ch = CHAR;
        if(ch != EOF) {
            if(ch == '%')
                get_directive(state);
            else
                get_rule(state);
        }
    }

    return state;
}
#endif
