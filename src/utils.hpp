#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <iostream>
#include <string>
#include <ranges>

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#ifdef _DEBUG
    #define DEBUG_LOG(fmt, ...) printf("[%s@%i]: "##fmt##"\n", __FILENAME__, __LINE__, __VA_ARGS__)
#else
    #define DEBUG_LOG()
#endif

static bool StringCompareI(std::string_view a, std::string_view b) {
    return std::ranges::equal(a, b, [](char a, char b) {
        return std::tolower(static_cast<int>(a)) == std::tolower(static_cast<int>(b));
    });
}


#endif // __UTILS_HPP__