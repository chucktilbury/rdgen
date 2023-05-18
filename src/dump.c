
#include "rdpgen.h"

static void dump_str_lst(str_lst_t* lst, const char* label, int verbo) {

    if(verbo > 5) {
        printf("\n----- %s -----\n", label);
        str_lst_reset(lst);
        for(Str* item = str_lst_iterate(lst); item != NULL; item = str_lst_iterate(lst))
            printf("%s\n", item->buffer);
    }
}

static void dump_rules(Pstate* state) {

    if(state->verbo > 3) {
        printf("----- rules -----\n\n");
        rule_lst_reset(state->rules);
        for(Rule* r = rule_lst_iterate(state->rules); r != NULL; r = rule_lst_iterate(state->rules)) {
            printf("%s\n", r->name->buffer);
            pattern_lst_reset(r->patterns);
            for(Pattern* p = pattern_lst_iterate(r->patterns); p != NULL; p = pattern_lst_iterate(r->patterns)) {
                printf("\t: ");
                str_lst_reset(p->lst);
                for(Str* s = str_lst_iterate(p->lst); s != NULL; s = str_lst_iterate(p->lst)) {
                    printf("%s ", s->buffer);
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

        dump_str_lst(state->parser_source, "psource", state->verbo);
        dump_str_lst(state->ast_source, "asource", state->verbo);
        dump_str_lst(state->parser_header, "pheader", state->verbo);
        dump_str_lst(state->ast_header, "aheader", state->verbo);
    }
}
