#include "browsermanager.hpp"

void browser_manager::Setup() {
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
}

void browser_manager::Start() {
    m_handle->Start();
}

browser::browser* browser_manager::GetHandle() {
    return m_handle.get();
}