#pragma once

#ifndef __CALLBACK_HPP__
#define __CALLBACK_HPP__

#include <functional>

// ~~ a class to contain multiple types of callbacks into 1 format

template <typename RTy, typename... Args>
class CallbackContainer {
public:
    // class callback
    template <typename C>
    using class_t = RTy(C::*)(Args...);

    // // function ptr callback
    // typedef RTy(*fnBasic)(Args...);

    // // lambda callback
    // typedef std::function<RTy(Args...)> fnLambda;

    CallbackContainer(void) = default;
    // CallbackContainer(fnBasic callback) { m_callback = callback; }
    // CallbackContainer(const fnLambda& callback) { m_callback = callback; }

    template <typename Ty>
    CallbackContainer(Ty&& callback) { m_callback = callback; }

    template <typename C>
    CallbackContainer(class_t<C> callback, C* obj) {
        m_callback = [callback, obj](Args&&... args) {
            if constexpr ((std::is_empty_v<Args> && ...)) {
                if constexpr (std::is_void_v<RTy>)
                    (obj->*callback)();
                else
                    return (obj->*callback)();
            } else {
                if constexpr (std::is_void_v<RTy>)
                    (obj->*callback)(std::forward<Args>(args)...);
                else
                    return (obj->*callback)(std::forward<Args>(args)...);
            }
        };
    }

    RTy Run(Args&&... args) {
        return m_callback(std::forward<Args>(args)...);
    }

    std::function<RTy(Args...)> GetCallback() {
        return m_callback;
    }

private:
    std::function<RTy(Args...)> m_callback = nullptr;
};

#endif // __CALLBACK_HPP__  