
#include "dump.h"
#include <stdio.h>

static void dump_str_lst(str_lst_t* lst, const char* label, int verbo) {

    if(verbo > 5) {
        printf("\n----- %s (%u) -----\n", label, lst->len);
        FOR_LST(Str, item, str_lst, lst)
            printf("%s\n", item->buffer);
    }
}

static void dump_rules(Pstate* state) {

    if(state->verbo > 3) {
        printf("----- rules (%u) -----\n\n", state->rules->len);
        FOR_LST(Rule, r, rule_lst, state->rules) {
            printf("%s - recursive:%s\n", r->name->buffer, r->is_recursive ? "true" : "false");
            FOR_LST(Pattern, p, pattern_lst, r->patterns) {
                printf("\t: ");
                FOR_LST(PatElem, pe, pat_elem_lst, p->elems) {
                    printf("%s ", pe->str->buffer);
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
        /*
        printf("---------- state ----------\n");
        printf("aname:  %s\n", state->ast_name->buffer);
        printf("pname:  %s\n\n", state->parser_name->buffer);
        */

        printf("\n");
        dump_rules(state);
        dump_str_lst(state->terminals, "terminals", 6);
        dump_str_lst(state->non_terminals, "non terminals", 6);

        dump_str_lst(state->parser_source, "psource", state->verbo);
        dump_str_lst(state->ast_source, "asource", state->verbo);
        dump_str_lst(state->parser_header, "pheader", state->verbo);
        dump_str_lst(state->ast_header, "aheader", state->verbo);

        printf("\n");
        printf("\n");
    }
}
