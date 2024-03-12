#include "checkbox.hpp"

ui::component_type ui::checkbox::GetType() const {
    return CT_CHECKBOX;
}

void ui::checkbox::Register(browser::browser* handle) {
    handle->CallJSFunction("uiCreateCheckBox", { m_label, m_state, GetId(), COMPONENT_CALLER_ID(HandleChange), m_target });
    handle->RegisterFunction(COMPONENT_CALLER_ID(HandleChange), &checkbox::HandleChange, this);
}

void ui::checkbox::Update(browser::browser*) {
    
}

void ui::checkbox::HandleChange(browser::browser*, browser::js_args_t args) {
    m_state = m_callbackHolder.Run(args.Get<0, bool>());
}