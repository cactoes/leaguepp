#pragma once

#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include <iostream>
#include <string>
#include <ranges>
#include <algorithm>
#include <charconv>
#include <vector>
#include <functional>

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

#define UNUSED [[maybe_unused]]

#ifdef _DEBUG
    #define DEBUG_LOG(fmt, ...) printf("[%s@%i]: "##fmt##"\n", __FILENAME__, __LINE__, __VA_ARGS__)
#else
    #define DEBUG_LOG(fmt, ...)
#endif

inline bool StringCompareI(std::string_view a, std::string_view b) {
    return std::ranges::equal(a, b, [](char a, char b) {
        return std::tolower(static_cast<int>(a)) == std::tolower(static_cast<int>(b));
    });
}

template <typename Ty>
inline bool ParseNumber(std::string_view str, Ty& value) {
    const auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value);
    return ec == std::errc();
}

template <typename Ta, typename Tb>
inline std::vector<Ta> ConvertAToB(const std::vector<Tb>& list, std::function<Ta(const Tb&)> convertor) {
    std::vector<Ta> out;
    for (const auto& v : list)
        out.push_back(convertor(v));
    return out;
}

#endif // __UTILS_HPP__