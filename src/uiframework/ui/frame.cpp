#include "frame.hpp"

#include "button.hpp"
#include "checkbox.hpp"
#include "dropdown.hpp"
#include "input.hpp"
#include "label.hpp"
#include "selector.hpp"
#include "slider.hpp"
#include "list.hpp"

ui::component_type ui::FrameImpl::GetType() const {
    return CT_FRAME;
}

void ui::FrameImpl::Register() {
    m_handle->CallJSFunction("uiCreateFrame", { m_name, GetId(), std::string{ m_layout }, m_target });

    for (const auto& comp : m_components)
        comp->Register();
}

void ui::FrameImpl::Update() {
    
}

std::shared_ptr<ui::Button> ui::FrameImpl::AddButton(const std::string& label, const button_callback& callbackHolder) {
    return AddComponent<ui::ButtonImpl>(label, callbackHolder, m_handle);
}

std::shared_ptr<ui::Checkbox> ui::FrameImpl::AddCheckbox(const std::string& label, bool state, const checkbox_callback& callbackHolder) {
    return AddComponent<ui::CheckboxImpl>(label, state, callbackHolder, m_handle);
}

std::shared_ptr<ui::Dropdown> ui::FrameImpl::AddDropdown(const std::string& label, bool isMulti, const std::vector<std::string>& activeValues, const std::vector<std::string>& values, const dropdown_callback& callbackHolder) {
    return AddComponent<ui::DropdownImpl>(label, isMulti, activeValues, values, callbackHolder, m_handle);
}

std::shared_ptr<ui::Input> ui::FrameImpl::AddInput(const std::string& label, const std::string& value, const input_callback& callbackHolder) {
    return AddComponent<ui::InputImpl>(label, value, callbackHolder, m_handle);
}

std::shared_ptr<ui::Label> ui::FrameImpl::AddLabel(const std::string& text, const std::string& pos) {
    return AddComponent<ui::LabelImpl>(text, pos, m_handle);
}

std::shared_ptr<ui::Selector> ui::FrameImpl::AddSelector(const std::string& label, int state, const std::vector<std::string>& values, const selector_callback& callbackHolder) {
    return AddComponent<ui::SelectorImpl>(label, state, values, callbackHolder, m_handle);
}

std::shared_ptr<ui::Slider> ui::FrameImpl::AddSlider(const std::string& label, int min, int max, const slider_callback& callbackHolder) {
    return AddComponent<ui::SliderImpl>(label, min, max,callbackHolder, m_handle);
}

std::shared_ptr<ui::List> ui::FrameImpl::AddList(const std::string& label, const std::vector<std::string>& activeValues, const list_validator_callback& validator, const list_callback& callbackHolder) {
    return AddComponent<ui::ListImpl>(label, activeValues, validator, callbackHolder, m_handle);
}

std::shared_ptr<ui::Frame> ui::FrameImpl::AddFrame(const std::string& name, const frame_layout& layout) {
    return AddComponent<ui::FrameImpl>(name, layout, m_handle);
}

void ui::FrameImpl::AddComponent(std::shared_ptr<Component> component) {
    component->m_target = GetId();
    AddComponent(std::move(component));
}