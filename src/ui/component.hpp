#ifndef __COMPONENT_HPP__
#define __COMPONENT_HPP__

#include <string>
#include <functional>
#include <type_traits>
#include <random>
#include "../browser/browser.hpp"

// GetId() is silent input
#define COMPONENT_CALLER_ID(callerType) GetId() + #callerType

namespace ui {
    enum component_type {
        CT_FRAME = 0,
        CT_BUTTON,
        CT_LABEL,
        CT_CHECKBOX,
        CT_SLIDER,
        CT_DROPDOWN,
        CT_SELECTOR
    };

    template <std::int32_t I>
    std::string RandomString() {
        const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, (int)charset.length() - 1);

        std::string randomString;
        randomString.reserve(I);

        for (int i = 0; i < I; ++i)
            randomString += charset[dis(gen)];

        return randomString;
    }

    template <typename... Args>
    class callback_holder {
    public:
        typedef void(*basic_t)(Args...);

        template <typename C>
        using class_t = void(C::*)(Args...);

        explicit callback_holder(basic_t callback) {
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

    class component {
    public:
        virtual component_type GetType() const = 0;
        virtual void Register(browser::browser* handle) = 0;

        const std::string& GetId() const { return m_id; }

    public:
        std::string m_target;

    private:
        std::string m_id = RandomString<16>();
    };
}; // namespace ui

#endif // __COMPONENT_HPP__