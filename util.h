#pragma once
#include "std.h"

namespace mtk {

    // implicit_cast<T>(...) is the same as static_cast<T>(...),
    // but only compiles if an implicit conversion to T is legal.
    template <typename D, typename S>
    constexpr inline D implicit_cast(S src) {
    #if defined(__clang__)
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wconversion"
    #endif
        auto identity = [](D dst){ return dst; };
        return identity(src);
    #if defined(__clang__)
        #pragma clang diagnostic pop
    #endif
    }
    static_assert(implicit_cast<float>(2   ) == 2.0f);
    static_assert(implicit_cast<int  >(2.0f) == 2   );


    // is_pow2_or_zero(n) returns true if n is zero or a power of 2.
    template <typename T>
    constexpr inline bool is_pow2_or_zero(T n) {
        return (n & (n-1)) == 0;
    }
    static_assert( is_pow2_or_zero(0));
    static_assert( is_pow2_or_zero(1));
    static_assert( is_pow2_or_zero(2));
    static_assert(!is_pow2_or_zero(3));
    static_assert( is_pow2_or_zero(4));
    static_assert(!is_pow2_or_zero(5));


    // push() and pop() maintain a dynamic array [ptr..ptr+len),
    // growing and shrinking exponentially to fit len elements.
    // The empty array is {ptr=nullptr, len=0}.
    template <typename T, typename Len>
    inline T& push(T* &ptr, Len &len) {
        if (is_pow2_or_zero(len)) {
            ptr = static_cast<T*>(realloc(ptr, implicit_cast<size_t>(len ? 2*len : 1) * sizeof(T)));
        }
        return ptr[len++];
    }
    template <typename T, typename Len>
    inline T pop(T* &ptr, Len &len) {
        T val = ptr[--len];
        if (is_pow2_or_zero(len)) {
            ptr = len ? static_cast<T*>(realloc(ptr, implicit_cast<size_t>(len) * sizeof(T)))
                      : (free(ptr), nullptr);
        }
        return val;
    }

}
