#ifndef __BUTTON_HPP__
#define __BUTTON_HPP__

#include "component.hpp"

namespace ui {
    // void ButtonCallback(void)
    typedef callback_holder<void> button_callback;

    class Button : public Component {
    public:
        Button(const std::string& label, const button_callback& callbackHolder, const std::string& target = "") :
            m_label(label), m_callbackHolder(callbackHolder) {
            m_target = target;
        }

        component_type GetType() const override;
        void Register(browser::Browser* handle) override;
        void Update(browser::Browser* handle) override;

        void HandleClick(browser::Browser*, browser::js_args_t);

    private:
        std::string m_label;
        button_callback m_callbackHolder;
    };
}; // namespace ui

#endif // __BUTTON_HPP__