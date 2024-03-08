#include "label.hpp"

ui::component_type ui::label::GetType() const {
    return CT_LABEL;
}

void ui::label::Register(browser::browser* handle) {
    handle->CallJSFunction("uiCreateLabel", { m_text, GetId(), m_target });
}

void ui::label::Update(browser::browser* handle) {
    handle->CallJSFunction("uiUpdateLabel", { m_text, GetId() });
}

void ui::label::SetText(const std::string& text) {
    m_text = text;
}