/**
 * @file list.c
 *
 * @brief Linked list implementation. This is a fairly complete implementation
 * that includes a stack and iteration interfaces. The list nodes are
 * accessible internally using tricky pointer arithmetic. This requires the
 * memory interface module and also supports GC.
 *
 */
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "list.h"
#include "memory.h"

/**
 * @brief Private data structure that holds the actual data.
 *
 */
typedef struct _list_elem_ {
    int type;
    size_t size;
    struct _list_elem_* next;
    struct _list_elem_* prev;
    // unsigned char data; is implied by the pointer arithmetic
} LST_ELEM;

/**
 * @brief Public data structure that implements lists.
 *
 */
struct _list_ {
    LST_ELEM* first;
    LST_ELEM* last;
    LST_ELEM* crnt;
};

#define CAST_LST(p) ((struct _list_*)(p))
#define VOID_TO_ELEM(p) ((LST_ELEM*)(((unsigned long)p) - sizeof(LST_ELEM)))
#define ELEM_TO_VOID(p) ((void*)(((unsigned long)p) + sizeof(LST_ELEM)))

#define RETURN_LST(p) return (LST)(p);

#define NOTNULL(p) assert((p) != NULL)
#define LOCAL(t, n, p) t* n = (t*)p
#define LOCAL_LST(n, p) \
    NOTNULL(p);         \
    LOCAL(struct _list_, n, p)
#define LOCAL_ELEM(n, p) \
    NOTNULL(p);          \
    LST_ELEM* n = VOID_TO_ELEM(p)

/**
 * @brief Create a list object
 *
 * @return LST
 */
LST create_lst() {

    struct _list_* lst = _alloc_obj(struct _list_);
    RETURN_LST(lst);
}

static void free_elem(LST_ELEM* elem) {

    if(elem != NULL) {
        _free(elem);
    }
}

/**
 * @brief Destroy a list object along with its data.
 *
 * @param lst
 */
void destroy_lst(LST lst) {

    LOCAL_LST(l, lst);

    LST_ELEM* elem;
    LST_ELEM* next;

    for(elem = l->first; elem != NULL; elem = next) {
        next = elem->next;
        free_elem(elem);
    }

    _free(lst);
}

static LST_ELEM* create_elem(void* data, size_t size, int type) {

    NOTNULL(data);
    LST_ELEM* elem = _alloc(sizeof(LST_ELEM) + size);
    elem->size     = size;
    elem->type     = type;
    memcpy(ELEM_TO_VOID(elem), data, size);

    // note that _alloc_obj() clears the memory.
    return elem;
}

/**
 * @brief Append data to the end of the list.
 *
 * @param lst
 * @param data
 * @param size
 * @param type
 */
void append_lst(LST lst, void* data, size_t size, int type) {

    NOTNULL(data);
    LOCAL_LST(l, lst);
    LST_ELEM* elem = create_elem(data, size, type);

    if(l->last != NULL) {
        elem->prev    = l->last;
        l->last->next = elem;
    }
    else {
        l->first = elem;
        l->crnt  = elem;
    }

    l->last = elem;
}

/**
 * @brief Add data to the beginning of the list.
 *
 * @param lst
 * @param data
 * @param size
 * @param type
 */
void prepend_lst(LST lst, void* data, size_t size, int type) {

    NOTNULL(data);
    LOCAL_LST(l, lst);
    LST_ELEM* elem = create_elem(data, size, type);

    if(l->first != NULL) {
        elem->next     = l->first;
        l->first->prev = elem;
    }
    else {
        l->last = elem;
        l->crnt = elem;
    }

    l->first = elem;
}

// if flag is true the insert after, else insert before
/**
 * @brief Insert data into the list, handling all of the cases of the insert
 * point being at the beginning or end of the list as well as if the list is
 * empty. If the flag is set, then the item is inserted after the given
 * element. If the flag is clear, then the new element is inserted where the
 * given list element is and that element is moved toward the end of the list.
 *
 * @param lst
 * @param elem
 * @param ptr
 * @param size
 * @param type
 * @param flag
 */
void insert_lst(LST lst, void* elem, void* ptr, size_t size, int type, bool flag) {

    NOTNULL(ptr);
    LOCAL_ELEM(e, elem);
    LST_ELEM* next;
    LST_ELEM* prev;

    // insert after node
    if(flag) {
        if(e->next == NULL) {
            append_lst(lst, ptr, size, type);
            return;
        }
        else {
            next = e->next;
            prev = e->next->prev;
        }
    }
    // insert before node
    else {
        if(e->prev == NULL) {
            prepend_lst(lst, ptr, size, type);
            return;
        }
        else {
            next = e->prev->next;
            prev = e->prev;
        }
    }

    LST_ELEM* nele = create_elem(ptr, size, type);
    nele->next     = next;
    nele->prev     = prev;

    next->prev = nele;
    prev->next = nele;
}

/**
 * @brief Insert the data given an integer index. If the index is negative,
 * then the count is from the end of the list instead of the beginning.
 *
 * @param lst
 * @param index
 * @param ptr
 * @param size
 * @param type
 * @param flag
 */
void insert_lst_idx(LST lst, int index, void* ptr, size_t size, int type, bool flag) {

    void* e = get_elem(lst, index);
    insert_lst(lst, e, ptr, size, type, flag);
}

/**
 * @brief Return a pointer to the element that is indicated by the index. If
 * the index is negative then count from the end of the list. For example, if
 * the index is -1 then return the last item in the list. If the index is 0
 * then return the first item of the list. If the list is empty then return
 * NULL.
 *
 * @param lst
 * @param index
 * @return void*
 */
void* get_elem(LST lst, int index) {

    LOCAL_LST(l, lst);
    LST_ELEM* e;

    int count = 0;
    if(index >= 0) {
        e = l->first;
        for(; e != NULL && count < index; count++)
            e = e->next;
    }
    else {
        e = l->last;
        for(count = -1; e != NULL && count > index; count--)
            e = e->prev;
    }

    if(e == NULL)
        return NULL;
    else
        return ELEM_TO_VOID(e);
}

/**
 * @brief If the element is a member of the list then this will return the
 * allocated size of the data. Otherwise, it returns garbage.
 *
 * @param elem
 * @return size_t
 */
size_t get_elem_size(void* elem) {

    NOTNULL(elem);
    return VOID_TO_ELEM(elem)->size;
}

/**
 * @brief Return the item's type that was specified when the item was created.
 *
 * @param elem
 * @return int
 */
int get_elem_type(void* elem) {

    NOTNULL(elem);
    return VOID_TO_ELEM(elem)->type;
}

/**
 * @brief Delete the specified item from the list, as specified using an
 * index. Does not account for crnt item, so items cannot be deleted while
 * iterating the list.
 *
 * @param lst
 * @param index
 */
void remove_lst_idx(LST lst, int index) {

    remove_lst(lst, (void*)get_elem(lst, index));
}

/**
 * @brief Delete the item as specified by the data element. Does not account
 * for the crnt element.
 *
 * @param lst
 * @param elem
 */
void remove_lst(LST lst, void* elem) {

    LOCAL_LST(l, lst);
    LOCAL_ELEM(e, elem);

    // first in a not-empty list
    if(e->prev == NULL && e->next != NULL) {
        l->first       = e->next;
        l->first->prev = NULL;
    }
    // last in a not-empty list
    else if(e->next == NULL && e->prev != NULL) {
        l->last       = e->prev;
        l->last->next = NULL;
    }
    // only element in the list
    else if(e->next == NULL && e->prev == NULL) {
        l->first = NULL;
        l->last  = NULL;
        l->crnt  = NULL;
    }
    // not first or last in a non-empty list
    else {
        // clarity is king
        LST_ELEM* next = e->next;
        LST_ELEM* prev = e->prev;
        next->prev     = prev;
        prev->next     = next;
    }
    free_elem(e);
}

/**
 * @brief Add the data to the beginning of the list as a stack.
 *
 * @param lst
 * @param ptr
 * @param size
 * @param type
 */
void push_lst(LST lst, void* ptr, size_t size, int type) {

    prepend_lst(lst, ptr, size, type);
}

/**
 * @brief Peek at the top of the stack and return the item there.
 *
 * @param lst
 * @return void*
 */
void* peek_lst(LST lst) {

    LOCAL_LST(l, lst);
    return ELEM_TO_VOID(l->first);
}

/**
 * @brief Remove the top item on the stack and destroy it.
 *
 * @param lst
 */
void pop_lst(LST lst) {

    LOCAL_LST(l, lst);
    LST_ELEM* tmp = l->first;
    l->first      = l->first->next;
    if(l->first != NULL)
        l->first->prev = NULL;
    else {
        l->last = NULL;
        l->crnt = NULL;
    }
    free_elem(tmp);
}

/**
 * @brief Reset the current pointer in the data structure to the beginning
 * of the list in preparation for iterating it. Use as the first expression
 * in a for() clause. Returns the first item in the list.
 *
 * @param lst
 * @return void*
 */
void* reset_lst(LST lst) {

    LOCAL_LST(l, lst);
    l->crnt = l->first;
    return ELEM_TO_VOID(l->crnt);
}

/**
 * @brief First increment the crnt pointer and then return the pointer. This
 * expects that reset was called before any call to iterate. It is not
 * possible to add or delete items from a list while iterating it. If that is
 * needed, then create a separate list and add items based on the criteria.
 *
 * @param lst
 * @return void*
 */
void* iterate_lst(LST lst) {

    LOCAL_LST(l, lst);
    if(l->crnt != NULL)
        l->crnt = l->crnt->next;

    if(l->crnt != NULL)
        return ELEM_TO_VOID(l->crnt);
    else
        return NULL;
}

/*
 * Highly contrived sanity test.
 */
#ifdef _TEST_LISTS_
// build string:
// gcc -Wall -Wextra -D_TEST_LISTS_ -g -o l list.c memory.c

typedef LST int_lst_t;

#define create() LST lst = create_lst()
#define destroy() destroy_lst(lst)

#define insert(i, f) insert_lst_idx(lst, i, &val, sizeof(int), 124, f)
#define remove(i) remove_lst_idx(lst, i)
#define push() push_lst(lst, &val, sizeof(int), 123)
#define pop() pop_lst(lst)
#define peek() peek_lst(lst)

#define reset() reset_lst(lst)
#define iterate() iterate_lst(lst)

#define show() show_list(lst)

void show_list(LST lst) {

    int count = 0;

    for(int* x = reset(); x != NULL; x = iterate())
        printf("%3d. value: %d (%d)\n", count++, *x, get_elem_type((void*)x));
    printf("\n");
}

int main() {

    create();
    int val = 0;

    val = 12;
    push();
    val = 1;
    push();
    val = 23;
    push();
    val = 82;
    push();
    val = 99;
    push();
    show();

    printf("insert 76 after index 2\n");
    val = 76;
    insert(2, true);
    show();

    printf("insert 54 at index 3\n");
    val = 54;
    insert(3, false);
    show();

    printf("remove index 0\n");
    remove(0);
    show();

    printf("remove index 5\n");
    remove(5);
    show();

    printf("remove index 3\n");
    remove(3);
    show();

    val = 34;
    printf("insert 34 at beginning\n");
    insert(0, 0);
    show();

    val = 43;
    printf("insert 43 at end\n");
    insert(-1, 1);
    show();

    destroy();
}

#endif
