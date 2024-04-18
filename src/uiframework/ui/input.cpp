#include "input.hpp"

ui::component_type ui::InputImpl::GetType() const {
    return CT_INPUT;
}

void ui::InputImpl::Register() {
    m_handle->CallJSFunction("uiCreateInput", { m_label, m_info, GetId(), m_value, COMPONENT_CALLER_ID(HandleChange), m_target });
    m_handle->RegisterFunction(COMPONENT_CALLER_ID(HandleChange), &InputImpl::HandleChange, this);
}

void ui::InputImpl::Update() {

}

void ui::InputImpl::HandleChange(browser::Browser*, browser::js_args_t args) {
    m_value = m_callbackHolder.Run(args.Get<0, std::string>());
}