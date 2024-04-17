#ifndef __BUTTON_HPP__
#define __BUTTON_HPP__

#include "../include/button.hpp"
#include "../browser/browser.hpp"

namespace ui {
    class ButtonImpl : public Button {
    public:
        ButtonImpl(const std::string& label, const button_callback& callbackHolder, browser::Browser* handle, const std::string& target = "") :
            m_label(label), m_callbackHolder(callbackHolder) {
            m_target = target;
            m_handle = handle;
        }

        component_type GetType() const override;
        void Update() override;
        void Register() override;

        void HandleClick(browser::Browser*, browser::js_args_t);

    private:
        std::string m_label;
        button_callback m_callbackHolder;
    };
}; // namespace ui

#endif // __BUTTON_HPP__