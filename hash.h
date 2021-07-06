#pragma once

typedef struct {
    int (*cmp)(const void*, const void*);
    struct { int hash,padding; const void* key; void* val; } *table;
    int len,cap;
} Hash;

void  insert(      Hash*, int hash, const void* key, void* val);
void* lookup(const Hash*, int hash, const void* key);
