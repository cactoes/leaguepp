#include "input.hpp"

ui::component_type ui::Input::GetType() const {
    return CT_INPUT;
}

void ui::Input::Register(browser::Browser* handle) {
    handle->CallJSFunction("uiCreateInput", { m_label, GetId(), m_value, COMPONENT_CALLER_ID(HandleChange), m_target });
    handle->RegisterFunction(COMPONENT_CALLER_ID(HandleChange), &Input::HandleChange, this);
}

void ui::Input::Update(browser::Browser*) {

}

void ui::Input::HandleChange(browser::Browser*, browser::js_args_t args) {
    m_value = m_callbackHolder.Run(args.Get<0, std::string>());
}