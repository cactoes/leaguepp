#pragma once

#ifndef __SINGLETON_HPP__
#define __SINGLETON_HPP__

#include <memory>

// ~~ Singleton class

template <typename Ty>
class Singleton {
public:
    Singleton(void) = default;
    ~Singleton(void) = default;

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    static Ty& Get(void) requires std::is_constructible_v<Ty> {
        static Ty instance {};
        return instance;
    }
};

#endif // __SINGLETON_HPP__