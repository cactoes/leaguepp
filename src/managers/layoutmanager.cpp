#include "layoutmanager.hpp"

#include "../interface/holder.hpp"
#include "configmanager.hpp"

// #include <iostream>

// void OnToggleCheckBox1(bool state) {
//     std::cout << "switched state to: " << state << "\n";
// }

// class testClass {
// public:
//     void OnClickButton() {
//         std::cout << "onclick\n";
//     }

//     void OnSliderChange(int value) {
//         std::cout << "new slider state: " << value << "\n";
//     }

//     void OnDropdownChange(std::string item, bool newItemState, std::vector<std::string> list) {
//         std::cout << "what: " << item << "\n";
//         std::cout << "newState: " << newItemState << "\n";

//         for (auto& it : list) {
//             std::cout << "item: " << it << "\n";
//         }
//     }

//     void SelectorChange(std::string current) {
//          std::cout << "current: " << current << "\n";
//     }
// };

//     static testClass g_tc{};

// basicFrame->AddComponent<ui::button>(std::make_shared<ui::button>("btn", ui::button_callback_t(&testClass::OnClickButton, &g_tc)));
// basicFrame->AddComponent<ui::label>("test label");
// basicFrame->AddComponent<ui::checkbox>("checkbox", ui::checkbox_callback_t(&OnToggleCheckBox1));
// basicFrame->AddComponent<ui::slider>("slider", 0, 100, ui::slider_callback_t(&testClass::OnSliderChange, &g_tc));
// basicFrame->AddComponent<ui::dropdown>("dropdown", std::vector<std::string>{ "opt1", "opt2", "opt3" }, ui::dropdown_callback_t(&testClass::OnDropdownChange, &g_tc));
// basicFrame->AddComponent<ui::selector>("selector", std::vector<std::string>{ "opt1", "opt2", "opt3" }, ui::selector_callback_t(&testClass::SelectorChange, &g_tc));

void layout_manager::Setup(browser::browser* handle) {
    handle->RegisterFunction("GetMainLayout", CREATE_REGISTRATION_MEMBER(layout_manager::PushLayout));

    m_frameMain = std::make_shared<ui::frame>("", ui::FL_HORIZONTAL, FRAME_TARGET_MAIN);
    m_frameMain->SetId("MainFrame");

    auto lobbyFrame = std::make_shared<ui::frame>("lobby", ui::FL_VERTICAL);
    lobbyFrame->SetId("LobbyFrame");

    // TODO: fix this syntax
    auto configManager = interface::GetHolder<config_manager>(from_singleton);
    auto cfg = configManager->GetConfig("cfg");

    lobbyFrame->AddComponent<ui::checkbox>("auto accept", cfg->GetVar<bool>("lobby::autoAccept"), ui::checkbox_callback_t([configManager, cfg](bool state) {
        cfg->SetVar("lobby::autoAccept", state);
        configManager->DumpConfig(cfg);
        return state;
    }));

    m_frameMain->AddComponent<ui::frame>(std::move(lobbyFrame));

    auto statsFrame = std::make_shared<ui::frame>("stats", ui::FL_VERTICAL);
    statsFrame->SetId("StatsFrame");
    m_frameMain->AddComponent<ui::frame>(std::move(statsFrame));
}

void layout_manager::PushLayout(browser::browser* handle, browser::js_args_t args) {
    m_frameMain->Register(handle);
}

std::shared_ptr<ui::frame> layout_manager::GetFrame() {
    return m_frameMain;
}