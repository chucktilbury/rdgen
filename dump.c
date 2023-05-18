
#include "rdgen.h"

static void dump_str_lst(str_lst_t* lst) {

    str_lst_reset(lst);
    for(Str* item = str_lst_iterate(lst); item != NULL; item = str_lst_iterate(lst))
        printf("%s\n", item->buffer);
}

static void dump_rules(Pstate* state) {

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

/*
 * Print the entire state to stdout.
 */
void dump_state(Pstate* state) {

    printf("---------- state ----------\n");
    printf("errors: %d\n", state->errors);
    printf("aname:  %s\n", state->ast_name->buffer);
    printf("pname:  %s\n\n", state->parser_name->buffer);

    dump_rules(state);

    printf("\n----- psource -----\n"); dump_str_lst(state->parser_source);
    printf("\n----- asource -----\n"); dump_str_lst(state->ast_source);
    printf("\n----- pheader -----\n"); dump_str_lst(state->parser_header);
    printf("\n----- aheader -----\n"); dump_str_lst(state->ast_header);

}
