#ifndef __CALLBACKHOLDER_HPP__
#define __CALLBACKHOLDER_HPP__

#include <functional>

template <typename... Args>
class callback_holder {
public:
    typedef void(*basic_t)(Args...);

    typedef std::function<void(Args...)> lambda_t;

    template <typename C>
    using class_t = void(C::*)(Args...);

    explicit callback_holder(basic_t callback) {
        m_callback = callback;
    }

    explicit callback_holder(lambda_t callback) {
        m_callback = callback;
    }

    template <typename C>
    explicit callback_holder(class_t<C> callback, C* obj) {
        m_callback = [callback, obj](Args&&... args) {
            if constexpr ((std::is_empty_v<Args> && ...)) {
                (obj->*callback)();
            } else {
                (obj->*callback)(std::forward<Args>(args)...);
            }
        };
    }

    void Run(Args&&... args) {
        m_callback(std::forward<Args>(args)...);
    }

private:
    std::function<void(Args...)> m_callback;
};

#endif // __CALLBACKHOLDER_HPP__