#ifndef __DROPDOWN_HPP__
#define __DROPDOWN_HPP__

#include <string>
#include <vector>
#include <any>

#include "component.hpp"

namespace ui {
    // void DropDownCallback(std::string item, bool newItemState, std::vector<std::string> list)
    typedef callback_holder<void, std::string, bool, std::vector<std::string>> dropdown_callback;

    class Dropdown : public Component {
    public:
        Dropdown(const std::string& label, const std::vector<std::string>& values, const dropdown_callback& callbackHolder, const std::string& target = "") :
            m_label(label), m_values(values), m_callbackHolder(callbackHolder) {
            m_target = target;
        }

        component_type GetType() const override;
        void Register(browser::Browser* handle) override;
        void Update(browser::Browser* handle) override;

        void HandleChange(browser::Browser*, browser::js_args_t args);

    private:
        std::string m_label;
        std::vector<std::string> m_values;
        dropdown_callback m_callbackHolder;
    };
}; // namespace ui

#endif // __DROPDOWN_HPP__