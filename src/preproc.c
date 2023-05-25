#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "errors.h"
#include "parser.h"


// if the string is not in the non-terminal list and the string is not
// already in the terminal list then return true.
static bool is_term(Pstate* state, Str* s) {

    if(!str_in_lst(state->terminals, s->buffer) && !str_in_lst(state->non_terminals, s->buffer))
        return true;
    else
        return false;
}

static void make_non_terms(Pstate* state) {

    FOR_LST(Rule, r, rule_lst, state->rules) {
        if(!str_in_lst(state->non_terminals, r->name->buffer))
            str_lst_add(state->non_terminals, r->name);
        else {
            SYNTAX("duplicate rule: %s", r->name->buffer);
        }
    }
}

static void make_terms(Pstate* state) {

    FOR_LST(Rule, r, rule_lst, state->rules) {
        FOR_LST(Pattern, p, pattern_lst, r->patterns) {
            FOR_LST(PatElem, pe, pat_elem_lst, p->elems) {
                // printf("%s ", s->buffer);
                if(is_term(state, pe->str)) {
                    str_lst_add(state->terminals, pe->str);
                    pe->is_terminal = true;
                }
            }
        }
    }
}

void pre_process(Pstate* state) {

    make_non_terms(state);
    make_terms(state);
}
