#ifndef _EMIT_H
#define _EMIT_H

#include "ptrlst.h"
#include "strs.h"

PTRLST_HEADER(estr_lst, Str*)

typedef struct {
    Str* name;
    estr_lst_t* lst;
} Emit;

Emit* create_emit(Str* name);
void emit(Emit* ptr, const char* fmt, ...);
void commit(Emit* ptr);

#endif
