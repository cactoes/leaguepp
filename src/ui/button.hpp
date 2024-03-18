#ifndef __BUTTON_HPP__
#define __BUTTON_HPP__

#include <string>

#include "component.hpp"

namespace ui {
    // void ButtonCallback(void)
    typedef callback_holder<void> button_callback_t;

    class button : public component {
    public:
        button(const std::string& label, const button_callback_t& callbackHolder, const std::string& target = "") :
            m_label(label), m_callbackHolder(callbackHolder) {
            m_target = target;
        }

        component_type GetType() const override;
        void Register(browser::browser* handle) override;
        void Update(browser::browser* handle) override;

        void HandleClick(browser::browser*, browser::js_args_t);

    private:
        std::string m_label;
        button_callback_t m_callbackHolder;
    };
}; // namespace ui

#endif // __BUTTON_HPP__