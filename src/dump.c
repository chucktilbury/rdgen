
#include "dump.h"
#include <stdio.h>

static void dump_str_lst(str_lst_t* lst, const char* label, int verbo) {

    if(verbo > 5) {
        printf("\n----- %s (%lu) -----\n", label, str_lst_len(lst));
        FOR_LST(STR, item, str_lst, lst)
        printf("%s\n", raw_str(item));
    }
}

static void dump_rules(Pstate* state) {

    if(state->verbo > 3) {
        printf("----- rules (%lu) -----\n\n", rule_lst_len(state->rules));
        FOR_LST(Rule, r, rule_lst, state->rules) {
            printf("%s - recursive:%s\n", raw_str(r->name), r->is_recursive ? "true" : "false");
            FOR_LST(Pattern, p, pattern_lst, r->patterns) {
                printf("\t: ");
                FOR_LST(PatElem, pe, pat_elem_lst, p->elems) {
                    printf("%s ", raw_str(pe->str));
                }
                printf("\n");
            }
            printf("\n");
        }
    }
}

/*
 * Print the entire state to stdout.
 */
void dump_state(Pstate* state) {

    if(state->verbo > 0) {
        printf("---------- state ----------\n");
        printf("%%name:   %s\n", raw_str(state->name));
        printf("%%prefix: %s\n", raw_str(state->prefix));
        printf("%%verbosity: %d\n\n", state->verbo);

        printf("\n");
        dump_rules(state);
        dump_str_lst(state->terminals, "terminals", state->verbo);
        dump_str_lst(state->non_terminals, "non terminals", state->verbo);

        dump_str_lst(state->code, "code", state->verbo);
        printf("\n");
        printf("\n");
    }
}
