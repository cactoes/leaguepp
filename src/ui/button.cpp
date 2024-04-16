#include "button.hpp"

ui::component_type ui::Button::GetType() const {
    return CT_BUTTON;
}

void ui::Button::Register(browser::Browser* handle) {
    handle->CallJSFunction("uiCreateButton", { m_label, GetId(), COMPONENT_CALLER_ID(HandleClick), m_target });
    handle->RegisterFunction(COMPONENT_CALLER_ID(HandleClick), &Button::HandleClick, this);
}

void ui::Button::Update(browser::Browser*) {
    
}

void ui::Button::HandleClick(browser::Browser *, browser::js_args_t) {
    return m_callbackHolder.Run();
}