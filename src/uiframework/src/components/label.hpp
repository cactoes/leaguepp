#pragma once

#ifndef __IMPL_LABEL_HPP__
#define __IMPL_LABEL_HPP__

#include "components/label.hpp"
#include "browser.hpp"

namespace component {
    class Label : public ILabel {
    public:
        Label(const std::string& name, Browser* browser, const std::string& target) {
            m_name = name;
            m_browser = browser;
            m_target = target;
        }

        void Update(void) override {
            m_browser->CallJSFunction("updateUIComponent", { "label", m_name, GetId() });
        }

        void Render(void) override {
            m_browser->CallJSFunction("createUIComponent", { "label", m_name, GetId(), m_target });
        }

        void SetText(const std::string& text) override {
            m_name = text;
            Update();
        }

    private:
        Browser* m_browser;
    };
}; // namespace component

#endif // __IMPL_LABEL_HPP__