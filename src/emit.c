
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "emit.h"
#include "preproc.h"

#define EMIT(fmt, ...) emit_str(fp, (fmt), ##__VA_ARGS__)

static FILE* open_outfile(Str* name) {
#if 0
    FILE* fp = fopen(name->buffer, "w");
#else
    (void)name;
    FILE*fp = stdout;
#endif

    return fp;
}

static void close_outfile(FILE* fp) {

    (void)fp;
}

static void emit_str(FILE* fp, const char* fmt, ...) {

    va_list args;

    va_start(args, fmt);
    vfprintf(fp, fmt, args);
    va_end(args);
}

static void emit_ast_types(FILE* fp, str_lst_t* lst) {

    EMIT("typedef enum {\n");
    FOR_LST(Str, s, str_lst, lst) {
        Str* tmp = copy_str(s);
        upcase_str(tmp);
        EMIT("  AST_%s,\n", tmp->buffer);
    }
    EMIT("} AstType;\n\n");
}

static void emit_typedefs(FILE* fp, str_lst_t* lst) {

    FOR_LST(Str, s, str_lst, lst)
        EMIT("typedef struct _%s_ %s_t;\n", s->buffer, s->buffer);

    EMIT("\n");
}

static void emit_data(FILE* fp, Rule* rule) {

    EMIT("struct _%s_ {\n  Ast ast;\n", rule->name->buffer);

    str_lst_t* stmp = str_lst_create();
    FOR_LST(Pattern, p, pattern_lst, rule->patterns) {
        // make a list of unique strings
        FOR_LST(PatElem, pe, pat_elem_lst, p->elems) {
            if(!str_in_lst(stmp, pe->str->buffer) && !pe->is_terminal)
                str_lst_add(stmp, pe->str);
        }
    }

    if(stmp->len > 0)
        // emit the unique list
        FOR_LST(Str, s, str_lst, stmp)
            EMIT("  %s_t* m_%s;\n", s->buffer, s->buffer);
    else
        EMIT("  TokenType type\n");

    EMIT("};\n\n");
}

static void emit_data_structures(FILE* fp, rule_lst_t* rules) {

    FOR_LST(Rule, r, rule_lst, rules)
        emit_data(fp, r);
}

static void ast_header(Pstate* state) {

    Str* tmp = create_str(state->ast_name->buffer);
    cat_str_str(tmp, ".h");

    FILE* fp = open_outfile(tmp);

    EMIT("// This file is generated: DO NOT EDIT\n");
    tmp = copy_str(state->ast_name);
    upcase_str(tmp);
    cat_str_str(tmp, "_H");
    EMIT("#ifndef _%s\n#define _%s\n\n", tmp->buffer, tmp->buffer);
    emit_ast_types(fp, state->non_terminals);
    emit_typedefs(fp, state->non_terminals);
    EMIT("typedef struct {\n  AstType type;\n} Ast;\n\n");
    emit_data_structures(fp, state->rules);

    EMIT("#endif\n\n");
    close_outfile(fp);
}

void emit(Pstate* state) {

    ast_header(state);
}
