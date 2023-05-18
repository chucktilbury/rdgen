
#include <stdio.h>
#include <stdarg.h>

#include "emit.h"

Emit* create_emit(Str* name) {

    Emit* ptr = _alloc_obj(Emit);
    ptr->name = name;
    ptr->lst = estr_lst_create();

    return ptr;
}

void emit(Emit* ptr, const char* fmt, ...) {

    va_list args;

    va_start(args, fmt);
    size_t len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    const char* str = _alloc_mem(len+1);
}

void commit(Emit* ptr) {
}
