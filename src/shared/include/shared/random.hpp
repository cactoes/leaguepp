#pragma once

#ifndef __RANDOM_HPP__
#define __RANDOM_HPP__

#include <string>
#include <random>

// ~~ random generation header
//    any kind of random items will be defined here

namespace random {
    template <int I = 8>
    inline std::string String(int count = I) {
        const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, (int)charset.length() - 1);

        std::string randomString;
        randomString.reserve(count);

        for (int i = 0; i < count; ++i)
            randomString += charset[dis(gen)];

        return randomString;
    }

    template <int Min = 0, int Max = 10>
    inline int Number(int min = Min, int max = Max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(min, max);
        return dis(gen);
    }

    inline std::string UUID() {
        return String(Number<4, 10>()) + "-" + String(Number<4, 10>()) + "-" + String(Number<4, 10>()) + "-" + String(Number<4, 10>());
    }
}; // namespace random

#endif // __RANDOM_HPP__