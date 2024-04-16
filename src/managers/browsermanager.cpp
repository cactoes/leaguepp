#include "browsermanager.hpp"

#include "../browser/browser.hpp"

bool BrowserManager::Init() {
    browser::window_config_t windowConfig = {};
    windowConfig.icon = 1;
    windowConfig.name = "league++";
    windowConfig.flags = browser::window_flags::WF_SHOW | browser::window_flags::WF_HIDE_TITLE_BAR;

    browser::browser_config_t browserConfig = {};

#ifdef _DEBUG
    SET_FLAG(browserConfig.flags, browser::browser_flags::BF_ALLOW_DEV_TOOLS);
    SET_FLAG(browserConfig.flags, browser::browser_flags::BF_ENABLE_DEBUG_CONSOLE);
#endif

    m_handle = browser::CreateBrowser(browserConfig, windowConfig);

    return true;
}

void BrowserManager::Start() {
    m_handle->Start();
}

browser::Browser* BrowserManager::GetHandle() {
    return m_handle.get();
}

void BrowserManager::CallJS(const std::string& name, const std::vector<std::any>& args) {
    m_handle->CallJSFunction(name, args);
}

void BrowserManager::CreateNotification(const std::string& title, const std::string& description, notification_type type) {
    m_handle->CallJSFunction("createNotification", { title, description, (int)type });
}