#ifndef __CHECKBOX_HPP__
#define __CHECKBOX_HPP__

#include <string>

#include "component.hpp"

namespace ui {
    typedef callback_holder<bool> checkbox_callback_t;

    class checkbox : public component {
    public:
        checkbox(const std::string& label, const checkbox_callback_t& callbackHolder, const std::string& target = "") :
            m_label(label), m_callbackHolder(callbackHolder) {
            m_target = target;
        }

        component_type GetType() const override;
        void Register(browser::browser* handle) override;

        void HandleChange(browser::browser*, browser::js_args_t args);

    private:
        std::string m_label;
        checkbox_callback_t m_callbackHolder;
    };
}; // namespace ui

#endif // __CHECKBOX_HPP__