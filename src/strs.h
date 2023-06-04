/**
 * @file strs.h
 *
 * @brief Routines to manipulate strings as an object, rather than as a raw
 * string in C. The string object is accessed through an opaque handle.
 *
 */
#ifndef _STRS_H
#define _STRS_H

#include <stddef.h> // for size_t type

/**
 * @brief Opaque handle for string object.
 *
 */
typedef char* STR;

/**
 * @brief Public interface.
 */
STR create_str(const char* str);
STR create_str_fmt(const char* fmt, ...);
int cat_str_char(STR h, int ch);
int cat_str_str(STR h, const char* str);
int cat_str_fmt(STR h, const char* fmt, ...);
STR strip_str(STR h);
int truncate_str(STR h, int idx);
int find_str(STR h, const char* s);
void clear_str(STR h);
int comp_str(STR h, const char* str);
STR clip_str(STR h, int start, int end);
void upcase_str(STR h);
void downcase_str(STR h);
STR copy_str(STR h);
const char* raw_str(STR h);
size_t len_str(STR h);

#endif
