#ifndef _PTRLST_H
#define _PTRLST_H

#include <stddef.h>

/*
typedef struct {
    void** list;
    size_t len;
    size_t cap;
    size_t idx;
} PtrLst;

PtrLst* create_lst();
void add_lst(PtrLst* lst, void* str);
void reset_lst(PtrLst* lst);
void* iterate_lst(PtrLst* lst);
*/

#define PTRLST_HEADER(NAME, TYPE) \
    typedef struct { \
        TYPE* list; \
        size_t len; \
        size_t cap; \
        size_t idx; \
    } NAME_t; \
        \
    NAME_t* NAME_create_lst(); \
    void NAME_add_lst(NAME_t* lst, TYPE* str); \
    void NAME_reset_lst(NAME_t* lst); \
    TYPE* NAME_iterate_lst(NAME_t* lst);

#define PTRLST_IMPL(NAME, TYPE) \
    #include "memory.h" \
     \
    NAME_t* NAME_create_lst() { \
     \
        NAME_t* lst = _alloc_obj(NAME_t); \
        lst->cap = 0x01 << 3;  \
        lst->list = _alloc_array(TYPE*, lst->cap); \
     \
        return lst; \
    } \
     \
    void NAME_add_lst(NAME_t* lst, TYPE* ptr) { \
     \
        if(lst->len+1 >= lst->cap) { \
            lst->cap <<= 1; \
            lst->list = _realloc_array(lst->list, TYPE*, lst->cap); \
        } \
     \
        lst->list[lst->len] = ptr; \
        lst->len++; \
    } \
     \
    void NAME_reset_lst(NAME_t* lst) { \
     \
        lst->idx = 0; \
    } \
     \
    TYPE* NAME_iterate_lst(NAME_t* lst) { \
     \
        TYPE* ptr = NULL; \
     \
        if(lst->idx < lst->len) { \
            ptr = lst->list[lst->idx]; \
            lst->idx++; \
        } \
     \
        return ptr; \
    }


#endif
