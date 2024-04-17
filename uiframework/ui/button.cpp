#include "button.hpp"

ui::component_type ui::ButtonImpl::GetType() const {
    return CT_BUTTON;
}

void ui::ButtonImpl::Register(browser::Browser* handle) {
    m_handle = handle;
    handle->CallJSFunction("uiCreateButton", { m_label, GetId(), COMPONENT_CALLER_ID(HandleClick), m_target });
    handle->RegisterFunction(COMPONENT_CALLER_ID(HandleClick), &ButtonImpl::HandleClick, this);
}

void ui::ButtonImpl::Update() {
    
}

void ui::ButtonImpl::HandleClick(browser::Browser *, browser::js_args_t) {
    return m_callbackHolder.Run();
}