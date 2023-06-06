#ifndef _FILEIO_H
#define _FILEIO_H

#include "strs.h"

// The file stack for input files is implicitly defined.
void open_input_file(const char* fname);
int get_char();
int consume_char();

int get_line_no();
int get_col_no();
const char* get_fname();

// Since multiple output files can be open in a moment, then it is accessed
// using an opaque handle.
typedef void* FPTR;
FPTR open_output_file(const char* fname);
void close_file(FPTR fp);

void emit_buf(FPTR h, const char* str);
void emit_fmt(FPTR h, const char* fmt, ...);
void emit_str(FPTR h, STR str);

#endif /* _FILEIO_H */
