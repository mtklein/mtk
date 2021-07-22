#pragma once

typedef struct {
    struct { int hash,val; } *table;
    int len,cap;
} Hash;

_Bool lookup(const Hash*, int hash, _Bool(*match)(int val, void* ctx), void* ctx, int* val);
void  insert(      Hash*, int hash, int val);
