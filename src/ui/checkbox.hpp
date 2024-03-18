#ifndef __CHECKBOX_HPP__
#define __CHECKBOX_HPP__

#include <string>

#include "component.hpp"

namespace ui {
    // bool CheckboxChange(bool newState)
    typedef callback_holder<bool, bool> checkbox_callback_t;

    class checkbox : public component {
    public:
        checkbox(const std::string& label, bool state, const checkbox_callback_t& callbackHolder, const std::string& target = "") :
            m_state(state), m_label(label), m_callbackHolder(callbackHolder) {
            m_target = target;
        }

        component_type GetType() const override;
        void Register(browser::browser* handle) override;
        void Update(browser::browser* handle) override;

        void HandleChange(browser::browser*, browser::js_args_t args);

    private:
        bool m_state;
        std::string m_label;
        checkbox_callback_t m_callbackHolder;
    };
}; // namespace ui

#endif // __CHECKBOX_HPP__