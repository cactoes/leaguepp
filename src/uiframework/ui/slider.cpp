#include "slider.hpp"

ui::component_type ui::SliderImpl::GetType() const {
    return CT_SLIDER;
}

void ui::SliderImpl::Register() {
    m_handle->CallJSFunction("uiCreateSlider", { m_label, GetId(), COMPONENT_CALLER_ID(HandleChange), m_min, m_max, m_target });
    m_handle->RegisterFunction(COMPONENT_CALLER_ID(HandleChange), &SliderImpl::HandleChange, this);
}

void ui::SliderImpl::Update() {
    
}

void ui::SliderImpl::HandleChange(browser::Browser*, browser::js_args_t args) {
    m_callbackHolder.Run(args.Get<0, int>());
}