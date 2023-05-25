#ifndef _EMIT_H
#define _EMIT_H

#include "parser.h"

#define EMIT(fmt, ...) emit_str((fmt), ##__VA_ARGS__)

void emit(Pstate* state);
void emit_str(const char* fmt, ...);
void close_outfile();
void open_outfile(Str* name);

#endif
