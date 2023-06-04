/**
 * @file list.h
 *
 * @brief List header. Fairly complete double linked list. This is intended
 * to be used with a pre-processor wrapper that automatically casts the
 * intended data back and forth. See the test code at the bottom of list.c
 * to see a simple contrived example example.
 */
#ifndef _LIST_H
#define _LIST_H

#include <stddef.h>

/**
 * @brief Lists are accessed using an opaque handle.
 */
typedef void* LST;

/**
 * @brief Public interface.
 */
LST create_lst();
void destroy_lst(LST lst);

void append_lst(LST lst, void* ptr, size_t size, int type);
void prepend_lst(LST lst, void* ptr, size_t size, int type);
void insert_lst_idx(LST lst, int index, void* ptr, size_t size, int type, bool flag);
void insert_lst(LST lst, void* elem, void* ptr, size_t size, int type, bool flag);

void* get_elem(LST lst, int index);
size_t get_elem_size(void* elem);
int get_elem_type(void* elem);

void remove_lst_idx(LST lst, int index);
void remove_lst(LST lst, void* elem);

void push_lst(LST lst, void* elem, size_t size, int type);
void pop_lst(LST lst);
void* peek_lst(LST lst);

void* reset_lst(LST lst);
void* iterate_lst(LST lst);

#endif /* _LIST_H */
