#ifndef __DROPDOWN_HPP__
#define __DROPDOWN_HPP__

#include "../include/dropdown.hpp"
#include "../browser/browser.hpp"

namespace ui {
    class DropdownImpl : public Dropdown {
    public:
        DropdownImpl(const std::string& label, const std::string& info, bool isMulti, const std::vector<std::string>& activeValues, const std::vector<std::string>& values, const dropdown_callback& callbackHolder, browser::Browser* handle, const std::string& target = "") :
            m_label(label), m_info(info), m_isMulti(isMulti), m_activeValues(activeValues), m_values(values), m_callbackHolder(callbackHolder) {
            m_target = target;
            m_handle = handle;
        }

        component_type GetType() const override;
        void Update() override;
        void Register() override;

        void HandleChange(browser::Browser*, browser::js_args_t args);

    private:
        std::string m_label;
        std::string m_info;
        bool m_isMulti;
        std::vector<std::string> m_activeValues;
        std::vector<std::string> m_values;
        dropdown_callback m_callbackHolder;
    };
}; // namespace ui

#endif // __DROPDOWN_HPP__