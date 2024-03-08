#include "frame.hpp"

ui::frame_layout::operator std::string() const  {
    switch (value) {
        case HORIZONTAL:
            return "horizontal";
        case VERTICAL:
            return "vertical";
        default:
            return "";
    }
}

ui::component_type ui::frame::GetType() const {
    return CT_FRAME;
}

void ui::frame::Register(browser::browser* handle) {
    handle->CallJSFunction("uiCreateFrame", { m_name, GetId(), std::string{ m_layout }, m_target });

    for (const auto& comp : m_children)
        comp->Register(handle);
}