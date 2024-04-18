#ifndef __LIST_HPP__
#define __LIST_HPP__

#include "../include/list.hpp"
#include "../browser/browser.hpp"

namespace ui {
    class ListImpl : public List {
    public:
        ListImpl(const std::string& label, const std::string& info, const std::vector<std::string>& activeValues, const list_validator_callback& validator, const list_callback& callbackHolder, browser::Browser* handle, const std::string& target = "") :
            m_label(label), m_info(info), m_activeValues(activeValues), m_validator(validator), m_callbackHolder(callbackHolder) {
            m_target = target;
            m_handle = handle;
        }

        component_type GetType() const override;
        void Update() override;
        void Register() override;

        void SetActiveItems(const std::vector<std::string>& items) override;

        bool HandleValidate(browser::Browser*, browser::js_args_t args);
        std::string HandleChange(browser::Browser*, browser::js_args_t args);

    private:
        std::string m_label;
        std::string m_info;
        std::vector<std::string> m_activeValues;
        list_validator_callback m_validator;
        list_callback m_callbackHolder;
    };
}; // namespace ui

#endif // __LIST_HPP__