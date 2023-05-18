#ifndef _PROCESS_H
#define _PROCESS_H

#include <stdbool.h>
#include "strs.h"
#include "ptrlst.h"

PTRLST_HEADER(term_lst, Str*)
PTRLST_HEADER(nterm_lst, Str*)
PTRLST_HEADER(pelem_lst, Str*)

typedef struct {
    bool is_empty;
    pelem_lst_t* lst;
} Predicate;

PTRLST_HEADER(pred_lst, Predicate*)

typedef struct {
    Str* name;
    bool is_recursive;
    bool has_nonterms;
    pred_lst_t* pred_lst;
} Rule;

PTRLST_HEADER(rule_lst, Rule*)

typedef struct {
    Str* afname;
    Str* pfname;
    rule_lst_t* rules;
    term_lst_t* terms;
    nterm_lst_t* nonterms;
} Process;

Process* create_process();

#endif /* _PROCESS_H */
