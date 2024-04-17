#ifndef __CHECKBOX_HPP__
#define __CHECKBOX_HPP__

#include "../include/checkbox.hpp"
#include "../browser/browser.hpp"

namespace ui {
    class CheckboxImpl : public Checkbox {
    public:
        CheckboxImpl(const std::string& label, bool state, const checkbox_callback& callbackHolder, const std::string& target = "") :
            m_state(state), m_label(label), m_callbackHolder(callbackHolder) {
            m_target = target;
        }

        component_type GetType() const override;
        void Update() override;
        void Register(browser::Browser* handle) override;

        void HandleChange(browser::Browser*, browser::js_args_t args);

    private:
        browser::Browser* m_handle;
        bool m_state;
        std::string m_label;
        checkbox_callback m_callbackHolder;
    };
}; // namespace ui

#endif // __CHECKBOX_HPP__