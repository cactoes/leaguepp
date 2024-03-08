#include "slider.hpp"

ui::component_type ui::slider::GetType() const {
    return CT_SLIDER;
}

void ui::slider::Register(browser::browser* handle) {
    handle->CallJSFunction("uiCreateSlider", { m_label, GetId(), COMPONENT_CALLER_ID(HandleChange), m_min, m_max, m_target });
    handle->RegisterFunction(COMPONENT_CALLER_ID(HandleChange), &slider::HandleChange, this);
}

void ui::slider::Update(browser::browser* handle) {
    
}

void ui::slider::HandleChange(browser::browser*, browser::js_args_t args) {
    m_callbackHolder.Run(args.Get<0, int>());
}