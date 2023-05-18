#ifndef _MEMORY_H
#define _MEMORY_H

#include <stddef.h>

void* mem_alloc(size_t size);
void* mem_realloc(void* ptr, size_t size);
void* mem_dup_str(const char* str);
void* mem_dup(void* ptr, size_t size);
void mem_free(void* ptr);

#define _alloc_mem(s) mem_alloc(s)
#define _alloc_obj(t) (t*)mem_alloc(sizeof(t))
#define _alloc_array(t, n) (t*)mem_alloc(sizeof(t) * (n))
#define _realloc_mem(p, s) mem_realloc((void*)(p), (s))
#define _realloc_obj(p, t) (t*)mem_realloc((void*)(p), sizeof(t))
#define _realloc_array(p, t, n) (t*)mem_realloc((void*)(p), sizeof(t) * (n))
#define _free(p) mem_free((void*)(p))
#define _dup_str(s) mem_dup_str(s)
#define _dup_mem(p, s) mem_dup((void*)(p), (s))
#define _dup_obj(p, t) (t*)mem_dup((void*)(p), sizeof(t))
#define _dup_array(p, t, n) (t*)mem_dup((void*)(p), sizeof(t) * (n))

#endif
