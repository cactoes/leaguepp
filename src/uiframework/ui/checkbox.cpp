#include "checkbox.hpp"

ui::component_type ui::CheckboxImpl::GetType() const {
    return CT_CHECKBOX;
}

void ui::CheckboxImpl::Register() {
    m_handle->CallJSFunction("uiCreateCheckBox", { m_label, m_info, m_state, GetId(), COMPONENT_CALLER_ID(HandleChange), m_target });
    m_handle->RegisterFunction(COMPONENT_CALLER_ID(HandleChange), &CheckboxImpl::HandleChange, this);
}

void ui::CheckboxImpl::Update() {
    
}

void ui::CheckboxImpl::HandleChange(browser::Browser*, browser::js_args_t args) {
    m_state = m_callbackHolder.Run(args.Get<0, bool>());
}