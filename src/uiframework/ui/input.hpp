#ifndef __INPUT_HPP__
#define __INPUT_HPP__

#include "../include/input.hpp"
#include "../browser/browser.hpp"

namespace ui {
    class InputImpl : public Input {
    public:
        InputImpl(const std::string& label, const std::string& value, const input_callback& callbackHolder, browser::Browser* handle, const std::string& target = "") :
            m_label(label), m_value(value), m_callbackHolder(callbackHolder) {
            m_target = target;
            m_handle = handle;
        }

        component_type GetType() const override;
        void Update() override;
        void Register() override;

        void HandleChange(browser::Browser*, browser::js_args_t args);

    private:
        std::string m_label;
        std::string m_value;
        input_callback m_callbackHolder;
    };
}; // namespace ui

#endif // __INPUT_HPP__