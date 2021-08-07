#pragma once

#include <stdbool.h>

typedef struct {
    struct { int hash,val; } *table;
    int len,cap;
} Hash;

bool lookup(const Hash*, int hash, bool(*match)(int val, void* ctx), void* ctx);
void insert(      Hash*, int hash, int val);
