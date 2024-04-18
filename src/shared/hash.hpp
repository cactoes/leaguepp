#ifndef __HASH_HPP__
#define __HASH_HPP__

#pragma warning(push)
#pragma warning(disable:4505)

#include <stdint.h>

namespace hash {
    static uint64_t Fnv1a64HashUnsafe(const char* str, uint64_t hash = 14695981039346656037ULL)  {
        while (*str != '\0') {
            hash ^= static_cast<uint64_t>(*str);
            hash *= 1099511628211ULL;
            ++str;
        }
        return hash;
    }

    static uint64_t Fnv1a64Hash(const char* str, uint64_t hash = 14695981039346656037ULL) {
        int i = 0;
        while (str[i] != '\0') {
            hash ^= static_cast<uint64_t>(str[i]);
            hash *= 1099511628211ULL;
            i++;
        }
        return hash;
    }

    constexpr uint64_t Fnv1a64HashConst(const char* str, uint64_t hash = 14695981039346656037ULL) {
        return (*str == '\0') ? hash :
            Fnv1a64HashConst(str + 1, (hash ^ static_cast<uint64_t>(*str)) * 1099511628211ULL);
    }

    constexpr uint64_t HashString64Const(const char* str, uint64_t hash = 0ULL) {
        return (*str == '\0') ? hash :
            HashString64Const(str + 1, (hash << 1) + static_cast<uint64_t>(*str));
    }
}; // namespace hash

#pragma warning(pop)

#endif // __HASH_HPP__