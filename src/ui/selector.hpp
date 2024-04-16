#ifndef __SELECTOR_HPP__
#define __SELECTOR_HPP__

#include <string>

#include "component.hpp"

namespace ui {
    // int SelectorCallback(std::string currentItem)
    typedef callback_holder<int, std::string> selector_callback;

    class Selector : public Component {
    public:
        Selector(const std::string& label, int state, const std::vector<std::string>& values, const selector_callback& callbackHolder, const std::string& target = "") :
            m_state(state), m_label(label), m_values(values), m_callbackHolder(callbackHolder) {
            m_target = target;
        }

        component_type GetType() const override;
        void Register(browser::Browser* handle) override;
        void Update(browser::Browser* handle) override;

        void HandleChange(browser::Browser*, browser::js_args_t args);

    private:
        int m_state;
        std::string m_label;
        std::vector<std::string> m_values;
        selector_callback m_callbackHolder;
    };
}; // namespace ui

#endif // __SELECTOR_HPP__