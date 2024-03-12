#include "dropdown.hpp"

ui::component_type ui::dropdown::GetType() const {
    return CT_DROPDOWN;
}

void ui::dropdown::Register(browser::browser* handle) {
    handle->CallJSFunction("uiCreateDropDown", { m_label, GetId(), COMPONENT_CALLER_ID(HandleChange), m_target, m_values });
    handle->RegisterFunction(COMPONENT_CALLER_ID(HandleChange), &dropdown::HandleChange, this);
}

void ui::dropdown::Update(browser::browser*) {
    
}

void ui::dropdown::HandleChange(browser::browser*, browser::js_args_t args) {
    std::vector<std::string> values;
    for (auto& item : args.GetArray<2>())
        values.push_back(item.As<std::string>());

    m_callbackHolder.Run(args.Get<0, std::string>(), args.Get<1, bool>(), std::move(values));
}