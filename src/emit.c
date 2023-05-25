
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>

#include "emit.h"
#include "preproc.h"
#include "emit_ast.h"
#include "emit_parser.h"
#include "errors.h"

#define TESTING 0

static FILE* fp = NULL;

void open_outfile(Str* name) {

#if TESTING == 0
    fp = fopen(name->buffer, "w");
    if(fp == NULL) {
        FATAL("cannot open output file: \"%s\": %s", name, strerror(errno));
    }
#else
    (void)name;
    fp = stdout;
#endif

}

void close_outfile() {

#if TESTING == 0
    fclose(fp);
#endif
}

void emit_str(const char* fmt, ...) {

    va_list args;

    va_start(args, fmt);
    vfprintf(fp, fmt, args);
    va_end(args);
}

void emit(Pstate* state) {

    if(get_errors() == 0) {
        emit_ast(state);
        emit_parser(state);
    }
}
