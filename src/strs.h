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
typedef void* STR;

/**
 * @brief Public interface.
 */
STR create_str(const char* str);
STR create_str_fmt(const char* fmt, ...);
void destroy_str(STR h);
int cat_str_char(STR h, int ch);
int cat_str_str(STR h, const char* str);
int cat_str_fmt(STR h, const char* fmt, ...);
int strip_str(STR h);

void clear_str(STR h);
void upcase_str(STR h);
void downcase_str(STR h);
STR copy_str(STR h);
int comp_str(STR h, const char* str);
size_t len_str(STR h);
const char* raw_str(STR h);

// int truncate_str(STR h, int idx);
// int find_str(STR h, const char* s);
// STR clip_str(STR h, int start, int end);

#endif
