#include "slider.hpp"

ui::component_type ui::Slider::GetType() const {
    return CT_SLIDER;
}

void ui::Slider::Register(browser::Browser* handle) {
    handle->CallJSFunction("uiCreateSlider", { m_label, GetId(), COMPONENT_CALLER_ID(HandleChange), m_min, m_max, m_target });
    handle->RegisterFunction(COMPONENT_CALLER_ID(HandleChange), &Slider::HandleChange, this);
}

void ui::Slider::Update(browser::Browser*) {
    
}

void ui::Slider::HandleChange(browser::Browser*, browser::js_args_t args) {
    m_callbackHolder.Run(args.Get<0, int>());
}