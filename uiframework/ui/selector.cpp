#include "selector.hpp"

ui::component_type ui::SelectorImpl::GetType() const {
    return CT_SELECTOR;
}

void ui::SelectorImpl::Register(browser::Browser* handle) {
    m_handle = handle;
    handle->CallJSFunction("uiCreateSelector", { m_label, m_state, GetId(), COMPONENT_CALLER_ID(HandleChange), m_target, m_values });
    handle->RegisterFunction(COMPONENT_CALLER_ID(HandleChange), &SelectorImpl::HandleChange, this);
}

void ui::SelectorImpl::Update() {
    
}

void ui::SelectorImpl::HandleChange(browser::Browser*, browser::js_args_t args) {
    m_state = m_callbackHolder.Run(args.Get<0, std::string>());
}