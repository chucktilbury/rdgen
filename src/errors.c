#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "errors.h"
#include "fileio.h"

static int errors = 0;

void syntax(const char* fmt, ...) {

    va_list args;

    fprintf(stderr, "syntax error: %s: %d: %d: ", get_fname(), get_line_no(), get_col_no());

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fputc('\n', stderr);
    errors++;
}

void fatal(const char* fmt, ...) {

    va_list args;

    fprintf(stderr, "fatal error: ");

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fputc('\n', stderr);
    errors++;
    exit(1);
}

int get_errors() {

    return errors;
}
