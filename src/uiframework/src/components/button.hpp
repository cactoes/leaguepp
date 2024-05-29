#pragma once

#ifndef __IMPL_BUTTON_HPP__
#define __IMPL_BUTTON_HPP__

#include "components/button.hpp"
#include "browser.hpp"

namespace component {
    class Button : public IButton {
    public:
        Button(const std::string& name, Browser* browser, const std::string& target, const FnButtonCallback& callback)
        : m_callback(callback) {
            m_name = name;
            m_browser = browser;
            m_target = target;
        }

        void Update(void) override {}

        void Render(void) override {
            m_browser->CallJSFunction("createUIComponent", { "button", m_name, GetId(), m_target, COMPONENT_CALLER_ID(OnCallback) });
            m_browser->RegisterFunction(COMPONENT_CALLER_ID(OnCallback), &Button::OnCallback, this);
        }

        void OnCallback(Browser*, JSArgs) {
            m_callback.Run();
        }

    private:
        Browser* m_browser;
        FnButtonCallback m_callback;
    };
}; // namespace component

#endif // __IMPL_BUTTON_HPP__