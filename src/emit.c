
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "emit.h"
#include "memory.h"
#include "ptrlst.h"
#include "strs.h"

PTRLST_IMPL(estr_lst, Str*)

Emit* create_emit(Str* name) {

    Emit* ptr = _alloc_obj(Emit);
    ptr->name = name;
    ptr->lst  = estr_lst_create();

    return ptr;
}

void emit(Emit* ptr, const char* fmt, ...) {

    va_list args;

    va_start(args, fmt);
    size_t len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    char* str = _alloc_mem(len + 1);

    va_start(args, fmt);
    vsnprintf(str, len + 1, fmt, args);
    va_end(args);

    estr_lst_add(ptr->lst, create_str(str));

    // free the string with GC
}

void commit(Emit* ptr) {

    FILE* fp = fopen(ptr->name->buffer, "w");

    if(fp == NULL) {
        fprintf(stderr, "fatal error: Cannot open output file: %s: %s\n", ptr->name->buffer, strerror(errno));
        exit(1);
    }

    estr_lst_reset(ptr->lst);
    for(Str* s = estr_lst_iterate(ptr->lst); s != NULL; s = estr_lst_iterate(ptr->lst))
        fprintf(fp, "%s\n", s->buffer);

    fclose(fp);
}
