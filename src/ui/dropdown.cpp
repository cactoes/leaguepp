#include "dropdown.hpp"

ui::component_type ui::Dropdown::GetType() const {
    return CT_DROPDOWN;
}

void ui::Dropdown::Register(browser::Browser* handle) {
    handle->CallJSFunction("uiCreateDropDown", { m_label, GetId(), m_isMulti, m_activeValues, COMPONENT_CALLER_ID(HandleChange), m_target, m_values });
    handle->RegisterFunction(COMPONENT_CALLER_ID(HandleChange), &Dropdown::HandleChange, this);
}

void ui::Dropdown::Update(browser::Browser*) {
    
}

void ui::Dropdown::HandleChange(browser::Browser*, browser::js_args_t args) {
    std::vector<std::string> values;
    for (auto& item : args.GetArray<2>())
        values.push_back(item.As<std::string>());

    m_activeValues = m_callbackHolder.Run(args.Get<0, std::string>(), args.Get<1, bool>(), std::move(values));
}