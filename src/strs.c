/**
 * @file strs.c
 *
 * @brief Various routines used to manipulate C strings as an object. These
 * strings are similar to strings in Pascal. The length is part of the string,
 * however, the string itself is still zero terminated like a C string. A data
 * structure is prepended to the string in order to track the memory allocated
 * for it.
 *
 */
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "strs.h"

/**
 * @brief Private string data structure.
 *
 */
struct _string_ {
    char* buffer;
    size_t len;
    size_t cap;
};

#define NOT_NULL(p) assert((p) != NULL)
#define LOCAL(n, p) \
    NOT_NULL(p);    \
    struct _string_* n = (struct _string_*)(p)

static void cat_string(struct _string_* ptr, const char* str) {

    size_t len = strlen(str);
    if(ptr->len + len + 1 > ptr->cap) {
        while(ptr->len + len + 1 > ptr->cap)
            ptr->cap <<= 1;
        ptr->buffer = _realloc_mem(ptr->buffer, ptr->cap);
    }

    memcpy(&ptr->buffer[ptr->len], str, len + 1);
    ptr->len += len;
    ptr->buffer[ptr->len] = '\0';
}

/**
 * @brief Create a Str object from a const char buffer.
 *
 * @param str
 * @return Str*
 */
STR create_str(const char* str) {

    struct _string_* ptr = _alloc_obj(struct _string_);
    ptr->cap             = 0x01 << 3;
    ptr->len             = 0;
    ptr->buffer          = _alloc_array(char, ptr->cap);

    if(str != NULL && strlen(str) > 0)
        cat_string(ptr, str);

    return (STR)ptr;
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

    char* str = _alloc(len + 2);

    va_start(args, fmt);
    vsnprintf(str, len + 2, fmt, args);
    va_end(args);

    STR ptr = create_str(str);
    _free(str);

    return ptr;
}

/**
 * @brief Destroy a string. This is a no-op if GC is active.
 *
 * @param h
 */
void destroy_str(STR h) {

    LOCAL(p, h);
    _free(p->buffer);
    _free(p);
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
        str->buffer = _realloc_array(str->buffer, char, str->cap);
    }

    str->buffer[str->len] = ch;
    str->len++;
    str->buffer[str->len] = '\0';

    return str->len;
}

/**
 * @brief Append a literal string to the end of a Str object.
 *
 * @param ptr
 * @param str
 * @return int
 */
int cat_str_str(STR ptr, const char* str) {

    LOCAL(p, ptr);
    size_t len = strlen(str);

    if(p->len + len + 1 >= p->cap) {
        while(p->len + len + 1 >= p->cap)
            p->cap <<= 1;
        p->buffer = _realloc_array(p->buffer, char, p->cap);
    }

    memcpy(&p->buffer[p->len], str, len + 1);
    p->len += len;
    p->buffer[p->len] = '\x0';

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

    char* str = _alloc(len + 2);

    va_start(args, fmt);
    vsnprintf(str, len + 2, fmt, args);
    va_end(args);

    cat_str_str(ptr, str);
    _free(str);

    return ((struct _string_*)ptr)->len;
}

/**
 * @brief Strip the spaces from the beginning and the end of a Str object.
 *
 * @param ptr
 * @return Str*
 */
int strip_str(STR ptr) {

    LOCAL(p, ptr);
    int idx;

    for(idx = 0; isspace(p->buffer[idx]) && p->buffer[idx] != '\x0'; idx++) {
    }

    if(idx > 0)
        memmove(p->buffer, &p->buffer[idx], strlen(&p->buffer[idx]) + 1);

    for(idx = strlen(p->buffer) - 1; isspace(p->buffer[idx]) && idx > 0; idx--)
        p->buffer[idx] = 0;

    p->len = strlen(ptr);
    return p->len;
}

/**
 * @brief Truncate the string to zero length. This does not free the string
 * buffer, but it does write a zero at the beginning of the string.
 *
 * @param str
 */
void clear_str(STR str) {

    LOCAL(ptr, str);
    ptr->len       = 0;
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
 * @brief Make string uppercase
 *
 * @param str
 */
void upcase_str(STR str) {

    for(char* ptr = ((struct _string_*)str)->buffer; *ptr != '\0'; ptr++)
        *ptr = toupper(*ptr);
}

/**
 * @brief Make string lower case.
 *
 * @param str
 */
void downcase_str(STR str) {

    for(char* ptr = ((struct _string_*)str)->buffer; *ptr != '\0'; ptr++)
        *ptr = tolower(*ptr);
}

/**
 * @brief Make a copy of the string.
 *
 * @param str
 * @return Str*
 */
STR copy_str(STR str) {

    return create_str(((struct _string_*)str)->buffer);
}

/**
 * @brief Return the number of characters in the string, excluding the
 * ending terminating null.
 *
 * @param ptr
 * @return size_t
 */
size_t len_str(STR ptr) {

    return ((struct _string_*)ptr)->len;
}

/**
 * @brief Return a pointer to the raw C string as a const char*.
 *
 * @param ptr
 * @return const char*
 */
const char* raw_str(STR ptr) {

    return ((struct _string_*)ptr)->buffer;
}


#ifdef TEST_STRINGS
// build string:
// gcc -Wall -Wextra -DTEST_STRINGS -g -o ts strs.c memory.c -lgc

int main() {

    STR s = create_str("create a string");
    printf("str: %s\n", raw_str(s));

    s = create_str_fmt("create a %s with %s", "string", "formatting");
    printf("str: %s\n", raw_str(s));

    cat_str_char(s, 'X');
    printf("str: %s\n", raw_str(s));

    cat_str_str(s, " <- an \"exx\"");
    printf("str: %s\n", raw_str(s));

    cat_str_fmt(s, " with %s with number: %d", "formatting", 1234);
    printf("str: %s\n", raw_str(s));

    s = create_str("  \t  this is before stripping    ");
    printf("str: \"%s\"\n", raw_str(s));
    strip_str(s);
    printf("str: \"%s\"\n", raw_str(s));
    clear_str(s);
    printf("str: \"%s\"\n", raw_str(s));

    cat_str_str(s, "this is a lower case string");
    printf("str: \"%s\"\n", raw_str(s));
    upcase_str(s);
    printf("str: \"%s\"\n", raw_str(s));
    downcase_str(s);
    printf("str: \"%s\"\n", raw_str(s));

    s = copy_str(s);
    printf("str: \"%s\"\n", raw_str(s));

    s = create_str("monkey patch the bacon");
    printf("compare strings \"%s\" and \"%s\" is %s\n", raw_str(s), "monkey patch the bacon",
           comp_str(s, "monkey patch the bacon") == 0 ? "true" : "false");

    printf("compare strings \"%s\" and \"%s\" is %s\n", raw_str(s), "monkey patch", comp_str(s, "monkey patch") == 0 ? "true" : "false");

    printf("compare strings \"%s\" and \"%s\" is %s\n", raw_str(s), "patch the bacon", comp_str(s, "patch the bacon") == 0 ? "true" : "false");

    printf("compare strings \"%s\" and \"%s\" is %s\n", raw_str(s), "Monkey patch", comp_str(s, "Monkey patch") == 0 ? "true" : "false");

    printf("string \"%s\" length is %lu\n", raw_str(s), len_str(s));

    return 0;
}

#endif

#if 0
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


#endif