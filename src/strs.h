#ifndef _STRS_H
#define _STRS_H

#include <stddef.h>

typedef struct {
    char* buffer;
    size_t len;
    size_t cap;
} Str;

Str* create_str(const char* str);
int cat_str_char(Str* ptr, int ch);
int cat_str_str(Str* ptr, const char* str);
Str* dup_str(Str* ptr);
Str* strip_str(Str* str);
int truncate_str(Str* str, int idx);
int find_str(Str* str, const char* s);
void clear_str(Str* str);
int comp_str(Str* base, const char* str);
Str* clip_str(Str* base, int start, int end);
void upcase_str(Str* str);
void downcase_str(Str* str);
Str* copy_str(Str* str);

#endif
