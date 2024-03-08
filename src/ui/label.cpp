#include "label.hpp"

ui::component_type ui::label::GetType() const {
    return CT_LABEL;
}

void ui::label::Register(browser::browser* handle) {
    handle->CallJSFunction("uiCreateLabel", { m_text, GetId(), m_target });
}