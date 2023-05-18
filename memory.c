
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#ifdef USE_GC
#include <gc.h>
#endif

void* mem_alloc(size_t size) {

#ifndef USE_GC
    void* ptr = malloc(size);
#else
    void* ptr = GC_MALLOC(size);
#endif

    if(ptr == NULL) {
        fprintf(stderr, "Cannot allocate memory: %lu\n", size);
        exit(1);
    }

    memset(ptr, 0, size);
    return ptr;
}

void* mem_realloc(void* ptr, size_t size) {

#ifndef USE_GC
    void* nptr = realloc(ptr, size);
#else
    void* nptr = GC_REALLOC(ptr, size);
#endif

    if(nptr == NULL) {
        fprintf(stderr, "Cannot reallocate memory: %lu\n", size);
        exit(1);
    }

    return nptr;
}

void mem_free(void* ptr) {

#ifndef USE_GC
    if(ptr != NULL)
        free(ptr);
    else {
        fprintf(stderr, "Attempt to free NULL pointer\n");
        exit(1);
    }
#else
    (void)ptr;
#endif
}

void* mem_dup_str(const char* str) {

    return mem_dup((void*)str, strlen(str) + 1);
}

void* mem_dup(void* ptr, size_t size) {

#ifndef USE_GC
    void* nptr = malloc(size);
#else
    void* nptr = GC_MALLOC(size);
#endif

    if(nptr == NULL) {
        fprintf(stderr, "Cannot allocate memory: %lu\n", size);
        exit(1);
    }

    memcpy(nptr, ptr, size);
    return nptr;
}
