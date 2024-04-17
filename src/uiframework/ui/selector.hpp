#ifndef __SELECTOR_HPP__
#define __SELECTOR_HPP__

#include "../include/selector.hpp"
#include "../browser/browser.hpp"

namespace ui {
    class SelectorImpl : public Selector {
    public:
        SelectorImpl(const std::string& label, int state, const std::vector<std::string>& values, const selector_callback& callbackHolder, const std::string& target = "") :
            m_state(state), m_label(label), m_values(values), m_callbackHolder(callbackHolder) {
            m_target = target;
        }

        component_type GetType() const override;
        void Update() override;
        void Register(browser::Browser* handle) override;

        void HandleChange(browser::Browser*, browser::js_args_t args);

    private:
        browser::Browser* m_handle;
        int m_state;
        std::string m_label;
        std::vector<std::string> m_values;
        selector_callback m_callbackHolder;
    };
}; // namespace ui

#endif // __SELECTOR_HPP__