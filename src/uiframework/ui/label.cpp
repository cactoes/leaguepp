#include "label.hpp"

ui::component_type ui::LabelImpl::GetType() const {
    return CT_LABEL;
}

void ui::LabelImpl::Register(browser::Browser* handle) {
    m_handle = handle;
    handle->CallJSFunction("uiCreateLabel", { m_text, m_pos, GetId(), m_target });
}

void ui::LabelImpl::Update() {
    m_handle->CallJSFunction("uiUpdateLabel", { m_text, GetId() });
}

void ui::LabelImpl::SetText(const std::string& text) {
    m_text = text;
}