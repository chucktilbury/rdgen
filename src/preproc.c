#include <stdbool.h>
#include <string.h>

#include "parser.h"
#include "errors.h"


// Returns true if the string is in the str_lst.
bool str_in_lst(str_lst_t* lst, const char* str) {

    Str* s = str_lst_reset(lst);
    while(s != NULL) {
        if(!strcmp(s->buffer, str))
            return true;
        s = str_lst_iterate(lst);
    }

    return false;
}

// if the string is not in the non-terminal list and the string is not
// already in the terminal list then return true.
static bool is_term(Pstate* state, Str* s) {

    if(!str_in_lst(state->terminals, s->buffer)
                && !str_in_lst(state->non_terminals, s->buffer))
        return true;
    else
        return false;
}

static void make_non_terms(Pstate* state) {

    rule_lst_reset(state->rules);
    for(Rule* r = rule_lst_iterate(state->rules); r != NULL; r = rule_lst_iterate(state->rules)) {
        if(!str_in_lst(state->non_terminals, r->name->buffer))
            str_lst_add(state->non_terminals, r->name);
        else {
            SYNTAX("duplicate rule: %s", r->name->buffer);
        }
    }
}

static void make_terms(Pstate* state) {

    rule_lst_reset(state->rules);
    for(Rule* r = rule_lst_iterate(state->rules); r != NULL; r = rule_lst_iterate(state->rules)) {
        pattern_lst_reset(r->patterns);
        for(Pattern* p = pattern_lst_iterate(r->patterns); p != NULL; p = pattern_lst_iterate(r->patterns)) {
            str_lst_reset(p->lst);
            for(Str* s = str_lst_iterate(p->lst); s != NULL; s = str_lst_iterate(p->lst)) {
                // printf("%s ", s->buffer);
                if(is_term(state, s))
                    str_lst_add(state->terminals, s);
            }
        }
    }
}

void pre_process(Pstate* state) {

    make_non_terms(state);
    make_terms(state);
}

