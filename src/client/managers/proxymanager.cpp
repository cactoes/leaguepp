#include "proxymanager.hpp"

#include "../interface/holder.hpp"
#include "../managers/configmanager.hpp"

#undef interface

bool ProxyManager::Init(IUiFramework* frameworkApiHandle) {
    m_configProxy = std::make_unique<proxy::ConfigProxy>(PROXY_PORT_CONFIG, PROXY_PORT_CHAT);
    m_configProxy->Start();

    m_chatProxy = std::make_unique<proxy::ChatProxy>(PROXY_PORT_CHAT);
    m_chatProxy->Start();

    SetupUi(frameworkApiHandle->AddTab("proxy")->GetFrame()->AddFrame("options", ui::FL_VERTICAL));
    return true;
}

void ProxyManager::Shutdown() {
    m_configProxy->Stop();
    m_chatProxy->Stop();
}

void ProxyManager::SetupUi(std::shared_ptr<ui::Frame> frame) {
    auto cfg = interface<ConfigManager>::Get()->Get(CONFIG_BASIC);
    frame->AddCheckbox(
        "appear offline", "", cfg->GetVar<bool>("proxy::appearOffline"),
        ui::checkbox_callback([this, cfg](bool state) {
            m_chatProxy->SetProxyState(state);
            return interface<ConfigManager>::Get()->TrackedSetVar(cfg, "proxy::appearOffline", state);
        })
    );
}
