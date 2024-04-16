#include "label.hpp"

ui::component_type ui::Label::GetType() const {
    return CT_LABEL;
}

void ui::Label::Register(browser::Browser* handle) {
    handle->CallJSFunction("uiCreateLabel", { m_text, m_pos, GetId(), m_target });
}

void ui::Label::Update(browser::Browser* handle) {
    handle->CallJSFunction("uiUpdateLabel", { m_text, GetId() });
}

void ui::Label::SetText(const std::string& text) {
    m_text = text;
}