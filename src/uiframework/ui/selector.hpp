#ifndef __SELECTOR_HPP__
#define __SELECTOR_HPP__

#include "../include/selector.hpp"
#include "../browser/browser.hpp"

namespace ui {
    class SelectorImpl : public Selector {
    public:
        SelectorImpl(const std::string& label, const std::string& info, int state, const std::vector<std::string>& values, const selector_callback& callbackHolder, browser::Browser* handle, const std::string& target = "") :
            m_state(state), m_label(label), m_info(info), m_values(values), m_callbackHolder(callbackHolder) {
            m_target = target;
            m_handle = handle;
        }

        component_type GetType() const override;
        void Update() override;
        void Register() override;

        void HandleChange(browser::Browser*, browser::js_args_t args);

    private:
        int m_state;
        std::string m_label;
        std::string m_info;
        std::vector<std::string> m_values;
        selector_callback m_callbackHolder;
    };
}; // namespace ui

#endif // __SELECTOR_HPP__