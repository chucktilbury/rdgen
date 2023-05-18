
#include "memory.h"
#include "ptrlst.h"

PtrLst* create_lst() {

    PtrLst* lst = _alloc_obj(PtrLst);
    lst->cap = 0x01 << 3;
    lst->list = _alloc_array(void*, lst->cap);

    return lst;
}

void add_lst(PtrLst* lst, void* ptr) {

    if(lst->len+1 >= lst->cap) {
        lst->cap <<= 1;
        lst->list = _realloc_array(lst->list, void*, lst->cap);
    }

    lst->list[lst->len] = ptr;
    lst->len++;
}

void reset_lst(PtrLst* lst) {

    lst->idx = 0;
}

void* iterate_lst(PtrLst* lst) {

    void* ptr = NULL;

    if(lst->idx < lst->len) {
        ptr = lst->list[lst->idx];
        lst->idx++;
    }

    return ptr;
}
