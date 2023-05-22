
#include "process.h"
#include "memory.h"
#include "rdpgen.h"

PTRLST_IMPL(term_lst, Str*)
PTRLST_IMPL(nterm_lst, Str*)
PTRLST_IMPL(pelem_lst, Str*)
PTRLST_IMPL(pred_lst, Predicate*)
PTRLST_IMPL(rule_lst, Rule*)


static Rule* copy_rule(PrimaryRule* rule) {

    Rule* r = _alloc_obj(Rule);

    r->name = create_str(rule->name->buffer);
    r->is_recursive = false;
    r->has_nonterms = false;

    pattern_lst_reset(rule->patterns);
    for(Pattern* p = pattern_lst_iterate(rule->patterns);
                p != NULL; p = pattern_lst_iterate(rule->patterns)) {

        str_lst_reset(p->lst);
        for(Str* s = str_lst_iterate(p->lst);
                    s != NULL; s = str_lst_iterate(p->lst)) {

        }

    }

    return r;
}

static rule_lst_t* convert_rules(Pstate* state) {

    rule_lst_t* lst = _alloc_obj(rule_lst_t);

    prim_rule_lst_reset(state->rules);
    for(prim_rule_lst_t* e = prim_rule_lst_iterate(state->rules);
                e != NULL; e = prim_rule_lst_iterate(state->rules)) {
        rule_lst_add(lst, copy_rule(e))
    }

    return lst;
}

static void convert_nterms(Process* ptr) {

}

static void convert_terms(Process* ptr) {

}

Process* create_process(Pstate* state) {

    Process* ptr = _alloc_obj(Process);
    ptr->afname = create_str(state->ast_name->buffer);
    ptr->pfname = create_str(state->parser_name->buffer);

    ptr->rules = convert_rules(state);
    convert_terms(ptr);
    convert_nterms(ptr);

    return ptr;
}
