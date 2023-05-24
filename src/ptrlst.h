#ifndef _PTRLST_H
#define _PTRLST_H

#define PTRLST_HEADER(N, T)          \
    typedef struct {                 \
        T* list;                     \
        unsigned int len;            \
        unsigned int cap;            \
        unsigned int idx;            \
    } N##_t;                         \
                                     \
    N##_t* N##_create();             \
    void N##_add(N##_t* lst, T ptr); \
    T N##_reset(N##_t* lst);         \
    T N##_iterate(N##_t* lst);       \
    T N##_get(N##_t* lst, unsigned int idx);

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
        if(lst->idx >= lst->len)                                \
            return NULL;                                        \
        else {                                                  \
            lst->idx++;                                         \
            return lst->list[lst->idx];                         \
        }                                                       \
    }                                                           \
                                                                \
    T N##_get(N##_t* lst, unsigned int idx) {                   \
                                                                \
        if(idx < lst->len)                                      \
            return lst->list[idx];                              \
        else                                                    \
            return NULL;                                        \
    }

/*
 * Example:
 *
 * FOR(Type, item, name_lst, list)
 *
 * Produces:
 *
 * for(Type* item = name_lst_reset(list);
 *          item != NULL;
 *          item = name_lst_iterate(list))
 *
 */
#define FOR_LST(T, I, N, L) for(T* I = N##_reset(L); I != NULL; I = N##_iterate(L))

/*
 * Example:
 *
 * WHILE(item, name_lst, list)
 *
 * Produces:
 *
 * item = name_lst_reset(list);
 * while(NULL != (item = name_lst_iterate(L)))
 *
 */
#define WHILE_LST(I, N, L) \
    I = N##_reset(L);      \
    while(NULL != (I = N##_iterate(L)))

#endif
