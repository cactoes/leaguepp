#ifndef __CHECKBOX_HPP__
#define __CHECKBOX_HPP__

#include "../include/checkbox.hpp"
#include "../browser/browser.hpp"

namespace ui {
    class CheckboxImpl : public Checkbox {
    public:
        CheckboxImpl(const std::string& label, const std::string& info, bool state, const checkbox_callback& callbackHolder, browser::Browser* handle, const std::string& target = "") :
            m_state(state), m_label(label), m_info(info), m_callbackHolder(callbackHolder) {
            m_target = target;
            m_handle = handle;
        }

        component_type GetType() const override;
        void Update() override;
        void Register() override;

        void HandleChange(browser::Browser*, browser::js_args_t args);

    private:
        bool m_state;
        std::string m_label;
        std::string m_info;
        checkbox_callback m_callbackHolder;
    };
}; // namespace ui

#endif // __CHECKBOX_HPP__