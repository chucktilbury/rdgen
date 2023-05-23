#ifndef _HASH_H_
#define _HASH_H_

#include <stdlib.h>

typedef enum {
    HASH_OK,
    HASH_DUP,
    HASH_NF,
} HashResult;

typedef struct {
    const char* key;
    void* data;
    size_t size;
} HashNode;

/*
 * If a node's key is NULL, but the bucket pointer in the table
 * is not NULL, then the bucket is a tombstone.
 */
typedef struct {
    HashNode** table;
    int cap;
    int count;
    int tombstones;
} Hash;

typedef void (*HashDumpFunc)(const char*, void*);

Hash* hashCreate();
void hashDestroy(Hash* table);
HashResult hashInsert(Hash* table, const char* key, void* data, size_t size);
HashResult hashFind(Hash* tab, const char* key, void* data, size_t size);
HashResult hashRemove(Hash* tab, const char* key);
void hashDump(Hash* tab, HashDumpFunc cb);

#endif
