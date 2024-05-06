#include "proxymanager.hpp"

#include <Windows.h>
#include <regex>
#include <utils.hpp>

#include "../interface/holder.hpp"
#include "configmanager.hpp"
#include "connectormanager.hpp"

#undef interface

bool ProxyManager::Init(IUiFramework* frameworkApiHandle) {
    m_configProxy = std::make_unique<proxy::ConfigProxy>(PROXY_PORT_CONFIG, PROXY_PORT_CHAT);
    m_configProxy->Start();

    m_chatProxy = std::make_unique<proxy::ChatProxy>(PROXY_PORT_CHAT);
    m_chatProxy->SetProxyState(interface<ConfigManager>::Get()->Get(CONFIG_BASIC)->GetVar<bool>("proxy::appearOffline"));
    m_chatProxy->Start();

    SetupUi(frameworkApiHandle);

    return true;
}

void ProxyManager::Shutdown() {
    m_configProxy->Stop();
    m_chatProxy->Stop();
}

void ProxyManager::UpdateLabel(bool state) {
    m_label->SetText(state ? "currently spoofing state" : "inactive");
    m_label->SetColor(state ? "9ecf8f" : "d17b7b");
}

void ProxyManager::SetupUi(IUiFramework* frameworkApiHandle) {
    auto proxyFrame = frameworkApiHandle->AddTab("proxy")->GetFrame();

    auto optionsFrame = proxyFrame->AddFrame("options", ui::FL_VERTICAL);
    auto clientFrame = proxyFrame->AddFrame("client", ui::FL_VERTICAL);

    m_label = optionsFrame->AddLabel("inactive", "left");

    auto cfg = interface<ConfigManager>::Get()->Get(CONFIG_BASIC);
    optionsFrame->AddCheckbox(
        "appear offline", "to update toggle your status ingame", cfg->GetVar<bool>("proxy::appearOffline"),
        ui::checkbox_callback([this, cfg](bool state) {
            m_chatProxy->SetProxyState(state);
            return interface<ConfigManager>::Get()->TrackedSetVar(cfg, "proxy::appearOffline", state);
        })
    );

    clientFrame->AddLabel("to start league and appear offline league++ needs to start the league of legends & riot client make sure both are closed", "left");

    clientFrame->AddInput(
        "riot client folder path", "", cfg->GetVar<std::string>("proxy::riotClientPath"),
        ui::input_callback([this, cfg](std::string input) {
            // format to only "\\"
            input = std::regex_replace(input, std::regex { "\\\\+|\\/+" }, "\\\\");
            // remove traling slashes
            input = std::regex_replace(input, std::regex { "(\\\\+|\\/+)$" }, "");

            return interface<ConfigManager>::Get()->TrackedSetVar(cfg,
                "proxy::riotClientPath", input);
        })
    );

    clientFrame->AddButton("start league",
        ui::button_callback([this, frameworkApiHandle, cfg]() {
            std::string path = cfg->GetVar<std::string>("proxy::riotClientPath");

            if (path.empty()) {
                frameworkApiHandle->CreateNotification("failed", "to launch the client no path was set");
                return;
            }

            HINSTANCE instance = ShellExecuteA(
                NULL, "open",
                (path + "\\\\RiotClientServices.exe").c_str(),
                ("--client-config-url=\"" + std::to_string(PROXY_PORT_CONFIG) + "\" --launch-product=league_of_legends --launch-patchline=live").c_str(),
                NULL, SW_SHOWDEFAULT
            );

            frameworkApiHandle->CreateNotification(
                (uint64_t)instance <= 32 ? "failed" : "launching",
                (uint64_t)instance <= 32 ? "to launch the client" : "the client is being launched"
            );
        })
    );
}
