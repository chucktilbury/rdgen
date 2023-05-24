
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "strs.h"

Str* create_str(const char* str) {

    Str* ptr    = _alloc_obj(Str);
    ptr->cap    = 0x01 << 3;
    ptr->len    = 0;
    ptr->buffer = _alloc_array(char, ptr->cap);

    if(str != NULL && strlen(str) > 0)
        cat_str_str(ptr, str);

    return ptr;
}

int cat_str_char(Str* str, int ch) {

    if(str->len + 1 >= str->cap) {
        str->cap <<= 1;
        str->buffer = _realloc_array(str->buffer, char, str->cap);
    }

    str->buffer[str->len] = ch;
    str->len++;
    str->buffer[str->len] = '\x0';

    return str->len;
}

int cat_str_str(Str* ptr, const char* str) {

    size_t len = strlen(str);

    if(ptr->len + len + 1 >= ptr->cap) {
        while(ptr->len + len + 1 >= ptr->cap)
            ptr->cap <<= 1;
        ptr->buffer = _realloc_array(ptr->buffer, char, ptr->cap);
    }

    memcpy(&ptr->buffer[ptr->len], str, len + 1);
    ptr->len += len;
    ptr->buffer[ptr->len] = '\x0';

    return ptr->len;
}

Str* dup_str(Str* ptr) {

    Str* str    = _dup_obj(ptr, Str);
    str->buffer = _alloc_array(char, str->cap);
    memcpy(str->buffer, ptr->buffer, ptr->len + 1);

    return str;
}

Str* strip_str(Str* ptr) {

    int idx;
    Str* dup = dup_str(ptr);

    // char* tmp = strchr(dup->buffer, '#');
    // if(tmp != NULL)
    //    *tmp = '\x0';

    char* tmp = dup->buffer;
    for(idx = 0; isspace(tmp[idx]) && tmp[idx] != '\x0'; idx++) {
    }

    if(idx > 0)
        memmove(tmp, &tmp[idx], strlen(&tmp[idx]) + 1);

    for(idx = strlen(tmp) - 1; isspace(tmp[idx]) && idx > 0; idx--)
        tmp[idx] = 0;

    dup->len = strlen(tmp);
    return dup;
}

int truncate_str(Str* str, int idx) {

    if((size_t)idx < str->len) {
        str->buffer[idx] = 0;
        str->len         = idx;
    }

    return str->len;
}

int find_str(Str* str, const char* s) {

    char* tmp = strstr(str->buffer, s);

    if(tmp != NULL)
        return (size_t)tmp - (size_t)str->buffer;
    else
        return -1;
}

void clear_str(Str* str) {

    str->len       = 0;
    str->buffer[0] = 0;
}

// returns 0 if the beginning of the base matches the str, regardless of what
// follows in the base.
int comp_str(Str* base, const char* str) {

    return strncmp(base->buffer, str, strlen(str));
}

// Return a new string that has the characters from the base between the
// start and the end indexes.
Str* clip_str(Str* base, int start, int end) {

    Str* retv = NULL;

    if(start < end && (size_t)start < base->len && (size_t)end < base->len) {
        retv = create_str(NULL);
        for(int idx = start; idx <= end; idx++)
            cat_str_char(retv, base->buffer[idx]);
    }

    return retv;
}

// Make string uppercase
void upcase_str(Str* str) {

    for(char* ptr = str->buffer; *ptr != '\0'; ptr++)
        *ptr = toupper(*ptr);
}

// Make string lower case
void downcase_str(Str* str) {

    for(char* ptr = str->buffer; *ptr != '\0'; ptr++)
        *ptr = tolower(*ptr);
}

// Make a copy of the string
Str* copy_str(Str* str) {

    return create_str(str->buffer);
}
