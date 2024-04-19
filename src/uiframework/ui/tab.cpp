#include "tab.hpp"

ui::component_type ui::TabImpl::GetType() const {
    return CT_TAB;
}

void ui::TabImpl::Update() {
    
}

void ui::TabImpl::Register() {
    m_handle->CallJSFunction("uiCreateTab", { m_label, m_isActive, GetId() });
    m_frame->Register();
}

std::shared_ptr<ui::Frame> ui::TabImpl::GetFrame() {
    return m_frame;
}

void ui::TabImpl::SetActive() {
    m_handle->CallJSFunction("uiSetActiveTab", { GetId() });
}
