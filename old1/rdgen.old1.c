
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>

#include "ptrlst.h"
#include "strs.h"
#include "memory.h"

typedef PtrLst StrLst;
typedef PtrLst RuleLst;

typedef struct {
    const char* name;
    PtrLst* patterns;
} Rule;

typedef struct {
    FILE* fh;
    Str* line;
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
} ParserState;

void consume_line(ParserState* state);

ParserState* create_parser(const char* fname) {

    ParserState* ptr = _alloc_obj(ParserState);

    ptr->fh = fopen(fname, "r");

    if(ptr->fh == NULL) {
        fprintf(stderr, "Cannot open input file: %s: %s\n", fname, strerror(errno));
        exit(1);
    }

    ptr->line_no = 1;
    ptr->col_no = 1;
    ptr->ast_source = create_lst();
    ptr->ast_header = create_lst();
    ptr->parser_source = create_lst();
    ptr->parser_header = create_lst();
    ptr->rules = create_lst();
    ptr->line = create_str(NULL);
    ptr->finished = false;

    ptr->ast_name = create_str(NULL);
    ptr->parser_name = create_str(NULL);

    // prime the stream of lines.
    consume_line(ptr);

    return ptr;
}

int get_char(ParserState* state) {

    int ch = fgetc(state->fh);

    if(ch != EOF) {
        if(ch == '\n') {
            state->line_no++;
            state->col_no = 1;
        }
        else
            state->col_no++;
    }
    else
        state->finished = true;

    return ch;
}

Str* get_line(ParserState* state) {

    return state->line;
}

const char* get_raw_line(ParserState* state) {

    return state->line->buffer;
}

bool is_finished(ParserState* state) {

    return state->finished;
}

void consume_line(ParserState* state) {

    clear_str(state->line);
    int ch = get_char(state);

    if(ch == EOF) {
        return;
    }
    else {
        while(1) {
            if(ch == '\n' || ch == EOF) {
                break;
            }
            else {
                cat_str_char(state->line, ch);
                ch = get_char(state);
            }
        }
    }

    // kill any comments in the line
    int idx = find_str(state->line, "#");

    if(idx >= 0)
        truncate_str(state->line, idx);
}

int read_char(ParserState* state) {
}

void do_name(ParserState* state, Str* str) {

    Str* cpy = dup_str(state->line);
    char* buf = cpy->buffer;

    char* start = strchr(buf, '\"');
    char* end = strrchr(buf, '\"');
    start++;

    if(start == end) {
        fprintf(stderr, "malformed name on line %d: %s\n", state->line_no, buf);
        exit(1);
    }

    while(start < end) {
        cat_str_char(str, *start);
        start++;
    }
}

int count_chars(const char* str, int ch) {

    int count = 0;

    for(int i = 0; str[i] != '\x0'; i++)
        if(str[i] == ch)
            count++;

    return count;
}

int counts(ParserState* state) {

    count = 0;

    count += count_chars(get_raw_line(state), '{');
    count -= count_chars(get_raw_line(state), '}');

    return count;
}

// When this is entered, the current line could have the opening curly or not.
// So find the opening curly first. The opening and closing curly are not
// included in the list of lines. All of the other lines are included verbatim.
void parse_directive(ParserState* state, StrLst* lst) {

    int count = 0;

}

void do_directive(ParserState* state) {

    StrLst* lst = NULL;

    if(comp_str(state->line, "%asource") == 0)
        lst = state->ast_source;
    else if(comp_str(state->line, "%psource") == 0)
        lst = state->parser_source;
    else if(comp_str(state->line, "%aheader") == 0)
        lst = state->ast_header;
    else if(comp_str(state->line, "%pheader") == 0)
        lst = state->parser_header;
    else if(comp_str(state->line, "%pname") == 0) {
        do_name(state, state->ast_name);
        return;
    }
    else if(comp_str(state->line, "%aname") == 0) {
        do_name(state, state->parser_name);
        return;
    }

    if(lst != NULL)
        parse_directive(state, lst);
    else {
        fprintf(stderr, "ERROR: Unknown directive on line %d: %s\n",
            state->line_no, state->line->buffer);
        exit(1);
    }
}

void* create_rule(ParserState* state) {

    return NULL;
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
        Str* tmp = strip_str(get_line(state));
        if(strlen(tmp->buffer) == 0) {
            consume_line(state);
            continue;
        }
        else if(tmp->buffer[0] == '%') {
            do_directive(state);
        }
        else {
           // add_lst(state->rules, create_rule(state));
        }

        consume_line(state);
    }

    dump_state(state);
    return 0;
}




#if 0
    for(Str* str = get_line(state); str != NULL; str = get_line(state)) {

        Str* tmp = kill_comment(str);
        tmp = strip_str(tmp);
        if(strlen(tmp->buffer) == 0)
            continue;
        else if(tmp->buffer[0] == '%')
            do_directive(tmp)
        else
            add_lst(lst, str);
    }

    reset_lst(lst);
    for(Str* str = iterate_lst(lst);
            str != NULL; str = iterate_lst(lst)) {

        printf(">> \"%s\"\n", str->buffer);
    }

    return 0;
}
#endif
