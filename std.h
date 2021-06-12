#pragma once

extern "C" {
    using size_t = decltype(sizeof(void*));

    void* realloc(void*, size_t);
    void free(void*);
}
