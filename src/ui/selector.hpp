#ifndef __SELECTOR_HPP__
#define __SELECTOR_HPP__

#include <string>

#include "component.hpp"

namespace ui {
    typedef callback_holder<int, std::string> selector_callback_t;

    class selector : public component {
    public:
        selector(const std::string& label, int state, const std::vector<std::string>& values, const selector_callback_t& callbackHolder, const std::string& target = "") :
            m_state(state), m_label(label), m_values(values), m_callbackHolder(callbackHolder) {
            m_target = target;
        }

        component_type GetType() const override;
        void Register(browser::browser* handle) override;
        void Update(browser::browser* handle) override;

        void HandleChange(browser::browser*, browser::js_args_t args);

    private:
        int m_state;
        std::string m_label;
        std::vector<std::string> m_values;
        selector_callback_t m_callbackHolder;
    };
}; // namespace ui

#endif // __SELECTOR_HPP__