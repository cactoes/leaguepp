#pragma once

#ifndef __IMPL_CHECKBOX_HPP__
#define __IMPL_CHECKBOX_HPP__

#include "components/checkbox.hpp"
#include "browser.hpp"

namespace component {
    class Checkbox : public ICheckbox {
    public:
        Checkbox(const std::string& name, Browser* browser, const std::string& target, bool state)
            : m_state(state) {
            m_name = name;
            m_browser = browser;
            m_target = target;
        }

        void Update(void) override {}

        void Render(void) override {
            m_browser->CallJSFunction("createUIComponent", { "checkbox", m_name, GetId(), m_target, m_state, COMPONENT_CALLER_ID(OnChange) });
            m_browser->RegisterFunction(COMPONENT_CALLER_ID(OnChange), &Checkbox::OnChange, this);
        }

        void OnChange(Browser*, JSArgs args) {
            m_state = args.at(0);
        }

        bool GetState(void) const override {
            return m_state;
        }

    private:
        Browser* m_browser;
        bool m_state;
    };
}; // namespace component

#endif // __IMPL_CHECKBOX_HPP__