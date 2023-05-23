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

#define PTRLST_HEADER(N, T)          \
    typedef struct {                 \
        T* list;                     \
        size_t len;                  \
        size_t cap;                  \
        size_t idx;                  \
    } N##_t;                         \
                                     \
    N##_t* N##_create();             \
    void N##_add(N##_t* lst, T ptr); \
    T N##_reset(N##_t* lst);         \
    T N##_iterate(N##_t* lst);

#define PTRLST_IMPL(N, T)                                       \
                                                                \
    N##_t* N##_create() {                                       \
                                                                \
        N##_t* lst = _alloc_obj(N##_t);                         \
        lst->cap   = 0x01 << 3;                                 \
        lst->list  = _alloc_array(T, lst->cap);                 \
                                                                \
        return lst;                                             \
    }                                                           \
                                                                \
    void N##_add(N##_t* lst, T ptr) {                           \
                                                                \
        if(lst->len + 1 >= lst->cap) {                          \
            lst->cap <<= 1;                                     \
            lst->list = _realloc_array(lst->list, T, lst->cap); \
        }                                                       \
                                                                \
        lst->list[lst->len] = ptr;                              \
        lst->len++;                                             \
    }                                                           \
                                                                \
    T N##_reset(N##_t* lst) {                                   \
                                                                \
        lst->idx = 0;                                           \
        return lst->list[0];                                    \
    }                                                           \
                                                                \
    T N##_iterate(N##_t* lst) {                                 \
                                                                \
        T ptr = NULL;                                           \
                                                                \
        if(lst->idx < lst->len) {                               \
            ptr = lst->list[lst->idx];                          \
            lst->idx++;                                         \
        }                                                       \
                                                                \
        return ptr;                                             \
    }


#endif
