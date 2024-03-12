#ifndef __DROPDOWN_HPP__
#define __DROPDOWN_HPP__

#include <string>
#include <vector>
#include <any>

#include "component.hpp"

namespace ui {
    typedef callback_holder<void, std::string, bool, std::vector<std::string>> dropdown_callback_t;

    class dropdown : public component {
    public:
        dropdown(const std::string& label, const std::vector<std::string>& values, const dropdown_callback_t& callbackHolder, const std::string& target = "") :
            m_label(label), m_values(values), m_callbackHolder(callbackHolder) {
            m_target = target;
        }

        component_type GetType() const override;
        void Register(browser::browser* handle) override;
        void Update(browser::browser* handle) override;

        void HandleChange(browser::browser*, browser::js_args_t args);

    private:
        std::string m_label;
        std::vector<std::string> m_values;
        dropdown_callback_t m_callbackHolder;
    };
}; // namespace ui

#endif // __DROPDOWN_HPP__