#ifndef _PTRLST_H
#define _PTRLST_H

#include "list.h"
/*
 * Never instantiate this.
 */
#define TDEF(N) typedef LST N##_t;

/*
 * Define this macro in the header file.
 */
#define PTRLST_HEADER(N, T)                  \
    TDEF(N)                                  \
    N##_t N##_create();                      \
    void N##_add(N##_t lst, T* ptr);         \
    T* N##_reset(N##_t lst);                 \
    T* N##_iterate(N##_t lst);               \
    T* N##_get(N##_t lst, unsigned int idx); \
    size_t N##_len(N##_t lst);

/*
 * Instantiate this macro in the source code module.
 */
#define PTRLST_IMPL(N, T)                               \
    N##_t N##_create() {                                \
        return (N##_t)create_lst();                     \
    }                                                   \
    void N##_add(N##_t lst, T* ptr) {                   \
        append_lst((LST)lst, (void*)ptr, sizeof(T), 0); \
    }                                                   \
    T* N##_reset(N##_t lst) {                           \
        return (T*)reset_lst((LST)lst);                 \
    }                                                   \
    T* N##_iterate(N##_t lst) {                         \
        return (T*)iterate_lst((LST)lst);               \
    }                                                   \
    T* N##_get(N##_t lst, unsigned int idx) {           \
        return (T*)get_elem((LST)lst, idx);             \
    }                                                   \
    size_t N##_len(N##_t lst) {                         \
        return get_lst_len((LST)lst);                   \
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
