

#include "utils.h"
#include "emit.h"
#include "emit_parser.h"

static Pstate* state = NULL;

static void emit_header() {

    Str* tmp = header_name(state->parser_name);
    open_outfile(tmp);
    unsigned count = 275;

    EMIT("// This file is generated: DO NOT EDIT\n");
    tmp = copy_str(state->parser_name);
    upcase_str(tmp);
    EMIT("#ifndef _%s_H\n#define _%s_H\n\n", tmp->buffer, tmp->buffer);
    EMIT("#include \"%s\"\n\n", header_name(state->ast_name)->buffer);
    EMIT("typedef enum {\n");
    FOR_LST(Str, s, str_lst, state->terminals)
        EMIT("  TOK_%s = %d,\n", s->buffer, count++);
    EMIT("} TokenType;\n\n");
    EMIT("Ast* parser(cont char* name);\n\n");
    EMIT("#endif /* _%s_H */\n\n", tmp->buffer);

    close_outfile();
}

static Str* emit_protos() {

    Rule* rule = rule_lst_reset(state->rules);
    EMIT("// prototypes\n");
    EMIT("Ast* %s();\n", rule->name->buffer);

    Str* tmp = create_str(rule->name->buffer);

    WHILE_LST(rule, rule_lst, state->rules) {
        EMIT("Ast* %s();\n", rule->name->buffer);
    }
    EMIT("\n");

    return tmp;
}

static void emit_rule_guts(Pattern* pat) {

    EMIT("  // ");
    FOR_LST(PatElem, pe, pat_elem_lst, pat->elems) {
        EMIT("%s ", pe->str->buffer);
    }
    EMIT("\n");
}

static void emit_single_rule(Rule* rule) {

    FOR_LST(Pattern, pat, pattern_lst, rule->patterns) {
        EMIT("static void* %s() {\n", rule->name->buffer);
        emit_rule_guts(pat);
        EMIT("}\n\n");
    }
    EMIT("\n");
}

static Str* emit_rules() {

    Str* tmp = emit_protos();
    EMIT("// function definitions\n");

    FOR_LST(Rule, rule, rule_lst, state->rules)
        emit_single_rule(rule);
    return tmp;
}

static void emit_includes() {

    EMIT("#include <stdio.h>\n");
    EMIT("#include <stdlib.h>\n");
    EMIT("#include <stddef.h>\n");
    EMIT("\n");
    EMIT("#include \"%s\"\n", header_name(state->parser_name)->buffer);
    EMIT("\n");
}

static void emit_source() {

    Str* tmp = source_name(state->parser_name);
    open_outfile(tmp);

    EMIT("// This file is generated: DO NOT EDIT\n");

    emit_includes();
    tmp = emit_rules();

    EMIT("// external interface\n");
    EMIT("Ast* parser(cont char* name) {\n");
    EMIT("  return %s();\n", tmp->buffer);
    EMIT("}\n");
    close_outfile();
}

void emit_parser(Pstate* s) {

    state = s;

    emit_header();
    emit_source();
}

