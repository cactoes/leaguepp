#include "button.hpp"

ui::component_type ui::button::GetType() const {
    return CT_BUTTON;
}

void ui::button::Register(browser::browser* handle) {
    handle->CallJSFunction("uiCreateButton", { m_label, GetId(), COMPONENT_CALLER_ID(HandleClick), m_target });
    handle->RegisterFunction(COMPONENT_CALLER_ID(HandleClick), &button::HandleClick, this);
}

void ui::button::Update(browser::browser* handle) {
    
}

void ui::button::HandleClick(browser::browser *, browser::js_args_t) {
    return m_callbackHolder.Run();
}