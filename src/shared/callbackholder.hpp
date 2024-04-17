#ifndef __CALLBACKHOLDER_HPP__
#define __CALLBACKHOLDER_HPP__

#include <functional>

template <typename RTy, typename... Args>
class callback_holder {
public:
    typedef RTy(*basic_t)(Args...);

    typedef std::function<RTy(Args...)> lambda_t;

    template <typename C>
    using class_t = RTy(C::*)(Args...);

    callback_holder() = default;

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

#endif // __CALLBACKHOLDER_HPP__