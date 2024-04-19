#include "framework_impl.hpp"

#include "ui/frame.hpp"
#include "ui/tab.hpp"

std::unique_ptr<IUiFramework> CreateFrameworkApi() {
    return std::make_unique<UiFrameworkImpl>();
}

bool UiFrameworkImpl::Init() {

    browser::window_config_t windowConfig = {};
    windowConfig.icon = 1;
    windowConfig.name = "league++";
    windowConfig.flags = browser::window_flags::WF_SHOW | browser::window_flags::WF_HIDE_TITLE_BAR;

    browser::browser_config_t browserConfig = {};

#ifdef _DEBUG
    SET_FLAG(browserConfig.flags, browser::browser_flags::BF_ALLOW_DEV_TOOLS);
    SET_FLAG(browserConfig.flags, browser::browser_flags::BF_ENABLE_DEBUG_CONSOLE);
#endif

    m_browserHandle = browser::CreateBrowser(browserConfig, windowConfig);
    
    m_browserHandle->RegisterFunction("GetMainLayout", CREATE_REGISTRATION_MEMBER(UiFrameworkImpl::GetMainLayout));

    return true; 
}

void UiFrameworkImpl::Shutdown() {

}

void UiFrameworkImpl::Run() {
    m_browserHandle->Start();
}

void UiFrameworkImpl::CreateNotification(const std::string& title, const std::string& description) {
    // enum class notification_type {
    //     NONE,
    //     ERR,
    //     WARNING,
    //     SUCCESS
    // };

    m_browserHandle->CallJSFunction("createNotification", { title, description, 0 });
}

std::vector<std::shared_ptr<ui::Tab>> UiFrameworkImpl::GetTabs() {
    return m_tabs;
}

std::shared_ptr<ui::Tab> UiFrameworkImpl::AddTab(const std::string& label, bool isActive) {
    auto tab = std::make_shared<ui::TabImpl>(label, isActive, m_browserHandle.get(), "");
    m_tabs.push_back(tab);
    return tab;
}

void UiFrameworkImpl::GetMainLayout(browser::Browser*, browser::js_args_t) {
    for (auto& tab : m_tabs)
        tab->Register();
}
