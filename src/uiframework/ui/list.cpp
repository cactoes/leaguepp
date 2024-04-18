#include "list.hpp"

ui::component_type ui::ListImpl::GetType() const {
    return CT_LIST;
}

void ui::ListImpl::Update() {
    m_handle->CallJSFunction("uiUpdateList", { GetId(), m_activeValues, COMPONENT_CALLER_ID(HandleValidate), COMPONENT_CALLER_ID(HandleChange) });
}

void ui::ListImpl::Register(){
    m_handle->CallJSFunction("uiCreateList", { m_label, GetId(), m_activeValues, COMPONENT_CALLER_ID(HandleValidate), COMPONENT_CALLER_ID(HandleChange), m_target });
    m_handle->RegisterFunction(COMPONENT_CALLER_ID(HandleValidate), &ListImpl::HandleValidate, this);
    m_handle->RegisterFunction(COMPONENT_CALLER_ID(HandleChange), &ListImpl::HandleChange, this);
}

void ui::ListImpl::SetActiveItems(const std::vector<std::string>& items) {
    m_activeValues = items;
    Update();
}

bool ui::ListImpl::HandleValidate(browser::Browser*, browser::js_args_t args) {
    return m_validator.Run(args.Get<0, std::string>());
}

std::string ui::ListImpl::HandleChange(browser::Browser*, browser::js_args_t args) {
    std::vector<std::string> values;
    for (auto& item : args.GetArray<0>())
        values.push_back(item.As<std::string>());

    m_activeValues = m_callbackHolder.Run(std::move(values));
    if (m_activeValues.size() == 0)
        return "";

    return m_activeValues.at(m_activeValues.size() - 1);
}