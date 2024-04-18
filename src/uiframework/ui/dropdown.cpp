#include "dropdown.hpp"

ui::component_type ui::DropdownImpl::GetType() const {
    return CT_DROPDOWN;
}

void ui::DropdownImpl::Register() {
    m_handle->CallJSFunction("uiCreateDropDown", { m_label, m_info, GetId(), m_isMulti, m_activeValues, COMPONENT_CALLER_ID(HandleChange), m_target, m_values });
    m_handle->RegisterFunction(COMPONENT_CALLER_ID(HandleChange), &DropdownImpl::HandleChange, this);
}

void ui::DropdownImpl::Update() {
    
}

void ui::DropdownImpl::HandleChange(browser::Browser*, browser::js_args_t args) {
    std::vector<std::string> values;
    for (auto& item : args.GetArray<2>())
        values.push_back(item.As<std::string>());

    m_activeValues = m_callbackHolder.Run(args.Get<0, std::string>(), args.Get<1, bool>(), std::move(values));
}