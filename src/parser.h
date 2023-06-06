#ifndef _PARSER_H
#define _PARSER_H

#include <stdbool.h>

#include "fileio.h"
#include "parser.h"
#include "ptrlst.h"
#include "strs.h"
#include "utils.h"

struct pat_elem_t {
    STR str;
    bool is_terminal;
};

struct pattern_t {
    // str_lst_t* lst;
    pat_elem_lst_t* elems;
    STR code; // C code to run upon a pattern match
};

struct rule_t {
    STR name;
    bool is_recursive;
    bool has_nonterms;
    pattern_lst_t* patterns;
};

typedef struct {

    bool finished;
    int verbo;

    // raw names of output files as given by directives
    STR ast_name;
    STR parser_name;

    // string lists that will be copied as source code to the output,
    // as given by directives
    str_lst_t* ast_source;
    str_lst_t* ast_header;
    str_lst_t* parser_source;
    str_lst_t* parser_header;

    // the raw rules as presented in the grammar
    rule_lst_t* rules;

    // list of all terminal and non-terminal words
    str_lst_t* terminals;
    str_lst_t* non_terminals;
} Pstate;

Pstate* parse_input(const char* fname);

#endif /* _PARSER_H */
