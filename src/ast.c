
#include "ast.h"
#include "emit.h"

void emit_ast(Pstate* state) {

    printf("emit ast: \"%s\"\n", state->ast_name->buffer);
}
