
#include "emit.h"
#include "emit_ast.h"
#include "utils.h"

static Pstate* state = NULL;

static void emit_ast_types(str_lst_t* lst) {

    EMIT("typedef enum {\n");
    FOR_LST(Str, s, str_lst, lst) {
        Str* tmp = copy_str(s);
        upcase_str(tmp);
        EMIT("  AST_%s,\n", tmp->buffer);
    }
    EMIT("} AstType;\n\n");
}

static void emit_typedefs(str_lst_t* lst) {

    EMIT("typedef struct {\n  AstType type;\n} Ast;\n\n");

    FOR_LST(Str, s, str_lst, lst)
        EMIT("typedef struct _%s_ %s_t;\n", s->buffer, s->buffer);

    EMIT("\n");
}

static void emit_data(Rule* rule) {

    EMIT("struct _%s_ {\n  Ast ast;\n", rule->name->buffer);

    str_lst_t* tmp = str_lst_create();
    FOR_LST(Pattern, p, pattern_lst, rule->patterns) {
        // make a list of unique strings
        FOR_LST(PatElem, pe, pat_elem_lst, p->elems) {
            if(str_in_lst(state->non_terminals, pe->str->buffer) &&
                        !str_in_lst(tmp, pe->str->buffer)) {
                str_lst_add(tmp, pe->str);
            }
        }
    }

    if(tmp->len == 0)
        EMIT("  TokenType type\n");
    else {
        FOR_LST(Str, s, str_lst, tmp) {
            EMIT("  %s_t* m_%s;\n", s->buffer, s->buffer);
        }
    }

    EMIT("};\n\n");
}

static void emit_data_structures() {

    FOR_LST(Rule, r, rule_lst, state->rules)
        emit_data(r);
}

static void ast_header() {

    Str* tmp = create_str(state->ast_name->buffer);
    cat_str_str(tmp, ".h");

    open_outfile(tmp);

    EMIT("// This file is generated: DO NOT EDIT\n");
    tmp = copy_str(state->ast_name);
    upcase_str(tmp);
    EMIT("#ifndef _%s_H\n#define _%s_H\n\n", tmp->buffer, tmp->buffer);

    emit_ast_types(state->non_terminals);
    emit_typedefs(state->non_terminals);
    emit_data_structures();

    EMIT("#endif /* _%s_H */\n\n", tmp->buffer);
    close_outfile();
}

void emit_ast(Pstate* s) {

    state = s;

    ast_header();
}

