/**
 * @file strs.c
 *
 * @brief Various routines used to manipulate C strings as an object.
 *
 */
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include "memory.h"
#include "strs.h"

/**
 * @brief Private string data structure.
 *
 */
struct _string_ {
    size_t len;
    size_t cap;
    // buffer is allocated behind this.
    char buffer[0]; // this is specific to GCC...
};

#define NOT_NULL(p)     assert((p)!=NULL)
#define LOCAL(n, p)     NOT_NULL(p);struct _string_*n = STR_TO_DS(p)
#define DS_TO_STR(s)  ((char*)((unsigned long)(s))+sizeof(struct _string_))
#define STR_TO_DS(s)  ((struct _string_*)((unsigned long)(s))-sizeof(struct _string_))
#define ALLOC_DS(cap)   (struct _string_*)_alloc_mem(sizeof(struct _string_)+cap+1)
#define REALLOC_DS(p)   (struct _string_*)_alloc_mem((p), sizeof(struct _string_)+(p)->cap+1)

static void cat_string(struct _string_* ptr, const char* str) {

    size_t len = strlen(str);
    if(ptr->len+len+1 > ptr->cap) {
        while(ptr->len+len+1 > ptr->cap)
            ptr->cap <<= 1;
        //ptr = _realloc_mem(ptr, sizeof(struct _string_)+ptr->cap);
        ptr = REALLOC_DS(ptr);
    }

    memcpy(&DS_TO_STR(ptr)[ptr->len], str, len+1);
    ptr->len += len;
    DS_TO_STR(ptr)[ptr->len] = '\0';
}

/**
 * @brief Create a Str object from a const char buffer.
 *
 * @param str
 * @return Str*
 */
STR create_str(const char* str) {

    unsigned cap = 0x01 << 3;
    struct _string_* ptr    = ALLOC_DS(cap);
    ptr->cap    = cap;
    ptr->len    = 0;

    if(str != NULL && strlen(str) > 0)
        cat_string(ptr, str);

    return DS_TO_STR(ptr);
}

/**
 * @brief Create a Str object using printf() style formatting.
 *
 * @param fmt
 * @param ...
 * @return Str*
 */
STR create_str_fmt(const char* fmt, ...) {

    va_list args;

    va_start(args, fmt);
    size_t len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    char* str = _alloc(len+2);

    va_start(args, fmt);
    vsnprintf(str, len+2, fmt, args);
    va_end(args);

    return DS_TO_STR(create_str(str));
}

/**
 * @brief Add a character to the end of a string.
 *
 * @param str
 * @param ch
 * @return int
 */
int cat_str_char(STR ptr, int ch) {

    LOCAL(str, ptr);
    if(str->len + 1 >= str->cap) {
        str->cap <<= 1;
        str = REALLOC_DS(str);
    }

    DS_TO_STR(str)[str->len] = ch;
    str->len++;
    DS_TO_STR(str)[str->len] = '\0';

    return str->len;
}

/**
 * @brief Append a literal string to the end of a Str object.
 *
 * @param ptr
 * @param str
 * @return int
 */
int cat_str_str(char* ptr, const char* str) {

    LOCAL(p, ptr);
    size_t len = strlen(str);

    if(p->len + len + 1 >= p->cap) {
        while(p->len + len + 1 >= p->cap)
            p->cap <<= 1;
        p = REALLOC_DS(p);
    }

    memcpy(&DS_TO_STR(p)[p->len], str, len + 1);
    p->len += len;
    DS_TO_STR(p)[p->len] = '\x0';

    return p->len;
}

/**
 * @brief Append a formatted string to the end of a Str object.
 *
 * @param ptr
 * @param fmt
 * @param ...
 * @return int
 */
int cat_str_fmt(STR ptr, const char* fmt, ...) {

    va_list args;

    va_start(args, fmt);
    size_t len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    char* str = _alloc(len+2);

    va_start(args, fmt);
    vsnprintf(str, len+2, fmt, args);
    va_end(args);

    // assumes garbage collection
    return cat_str_str(ptr, str);
}

/**
 * @brief Strip the spaces from the beginning and the end of a Str object.
 *
 * @param ptr
 * @return Str*
 */
STR strip_str(STR ptr) {

    int idx;

    for(idx = 0; isspace(ptr[idx]) && ptr[idx] != '\x0'; idx++) {
    }

    if(idx > 0)
        memmove(ptr, &ptr[idx], strlen(&ptr[idx]) + 1);

    for(idx = strlen(ptr) - 1; isspace(ptr[idx]) && idx > 0; idx--)
        ptr[idx] = 0;

    STR_TO_DS(ptr)->len = strlen(ptr);
    return ptr;
}

/**
 * @brief Shorten the string to the specified index where the character at
 * the index is the terminating zero of the string.
 *
 * @param str
 * @param idx
 * @return int
 */
int truncate_str(STR str, int idx) {

    LOCAL(ptr, str);
    if((size_t)idx < ptr->len) {
        ptr->len = idx;
        ptr->buffer[ptr->len] = '\0';
    }

    return ptr->len;
}

/**
 * @brief Return the index of the first instance of the substring. If the
 * substring is not found then return a negative value.
 *
 * @param str
 * @param s
 * @return int
 */
int find_str(STR str, const char* s) {

    LOCAL(ptr, str);
    char* tmp = strstr(ptr->buffer, s);

    if(tmp != NULL)
        return (size_t)tmp - (size_t)ptr->buffer;
    else
        return -1;
}

/**
 * @brief Truncate the string to zero length. This does not free the string
 * buffer, but it does write a zero at the beginning of the string.
 *
 * @param str
 */
void clear_str(STR str) {

    LOCAL(ptr, str);
    ptr->len = 0;
    ptr->buffer[0] = 0;
}

/**
 * @brief Compares the literal string to the string object. If the object is
 * longer, then only compare the length of the literal string. If the compare
 * matches, then return zero, otherwise return a positive or negative value
 * same as strcmp() does.
 *
 * @param base
 * @param str
 * @return int
 */
int comp_str(STR base, const char* str) {

    LOCAL(ptr, base);
    return strncmp(ptr->buffer, str, strlen(str));
}

/**
 * @brief Return a new string that contains the characters from the base
 * string that falls between the indexes, inclusive. If there are no
 * characters within the range, then return NULL.
 *
 * @param base
 * @param start
 * @param end
 * @return Str*
 */
STR clip_str(STR base, int start, int end) {

    LOCAL(ptr, base);
    STR retv = NULL;

    if(start < end && (size_t)start < base->len && (size_t)end < base->len) {
        retv = create_str(NULL);
        for(int idx = start; idx <= end; idx++)
            cat_str_char(retv, base->buffer[idx]);
    }

    return retv;
}

/**
 * @brief Make string uppercase
 *
 * @param str
 */
void upcase_str(Str* str) {

    for(char* ptr = str->buffer; *ptr != '\0'; ptr++)
        *ptr = toupper(*ptr);
}

/**
 * @brief Make string lower case.
 *
 * @param str
 */
void downcase_str(Str* str) {

    for(char* ptr = str->buffer; *ptr != '\0'; ptr++)
        *ptr = tolower(*ptr);
}

/**
 * @brief Make a copy of the string.
 *
 * @param str
 * @return Str*
 */
Str* copy_str(Str* str) {

    return create_str(str->buffer);
}

/**
 * @brief Return a pointer to the raw C string as a const char*.
 *
 * @param ptr
 * @return const char*
 */
const char* raw_str(Str* ptr) {

    return ptr->buffer;
}

/**
 * @brief Return the number of characters in the string, excluding the
 * ending terminating null.
 *
 * @param ptr
 * @return size_t
 */
size_t len_str(Str* ptr) {

    return ptr->len;
}
