#include "checkbox.hpp"

ui::component_type ui::Checkbox::GetType() const {
    return CT_CHECKBOX;
}

void ui::Checkbox::Register(browser::Browser* handle) {
    handle->CallJSFunction("uiCreateCheckBox", { m_label, m_state, GetId(), COMPONENT_CALLER_ID(HandleChange), m_target });
    handle->RegisterFunction(COMPONENT_CALLER_ID(HandleChange), &Checkbox::HandleChange, this);
}

void ui::Checkbox::Update(browser::Browser*) {
    
}

void ui::Checkbox::HandleChange(browser::Browser*, browser::js_args_t args) {
    m_state = m_callbackHolder.Run(args.Get<0, bool>());
}