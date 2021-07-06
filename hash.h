#pragma once

typedef struct {
    _Bool (*eq)(const void*, const void*, void* ctx);
    void* ctx;
    struct { int hash,padding; const void* key; void* val; } *table;
    int len,cap;
} Hash;

void  insert(      Hash*, int hash, const void* key, void* val);
void* lookup(const Hash*, int hash, const void* key);
