#ifndef _RDGEN_H
#define _RDGEN_H

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#include "ptrlst.h"
#include "strs.h"

typedef struct pattern_t Pattern;
typedef struct rule_t Rule;

PTRLST_HEADER(str_lst, Str*)
PTRLST_HEADER(rule_lst, Rule*)
PTRLST_HEADER(pattern_lst, Pattern*)

struct pattern_t {
    str_lst_t* lst;
};

struct rule_t {
    Str* name;
    pattern_lst_t* patterns;
};

typedef struct {
    FILE* fh;
    int ch;
    int line_no;
    int col_no;
    bool finished;
    int errors;
    Str* ast_name;
    Str* parser_name;
    str_lst_t* ast_source;
    str_lst_t* ast_header;
    str_lst_t* parser_source;
    str_lst_t* parser_header;
    rule_lst_t* rules;
} Pstate;


#endif
