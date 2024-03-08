#include "selector.hpp"

ui::component_type ui::selector::GetType() const {
    return CT_SELECTOR;
}

void ui::selector::Register(browser::browser* handle) {
    handle->CallJSFunction("uiCreateSelector", { m_label, GetId(), COMPONENT_CALLER_ID(HandleChange), m_target, m_values });
    handle->RegisterFunction(COMPONENT_CALLER_ID(HandleChange), &selector::HandleChange, this);
}

void ui::selector::HandleChange(browser::browser*, browser::js_args_t args) {
    m_callbackHolder.Run(args.Get<0, std::string>());
}