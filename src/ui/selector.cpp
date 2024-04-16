#include "selector.hpp"

ui::component_type ui::Selector::GetType() const {
    return CT_SELECTOR;
}

void ui::Selector::Register(browser::Browser* handle) {
    handle->CallJSFunction("uiCreateSelector", { m_label, m_state, GetId(), COMPONENT_CALLER_ID(HandleChange), m_target, m_values });
    handle->RegisterFunction(COMPONENT_CALLER_ID(HandleChange), &Selector::HandleChange, this);
}

void ui::Selector::Update(browser::Browser*) {
    
}

void ui::Selector::HandleChange(browser::Browser*, browser::js_args_t args) {
    m_state = m_callbackHolder.Run(args.Get<0, std::string>());
}