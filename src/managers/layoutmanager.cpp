#include "layoutmanager.hpp"

#include "../interface/holder.hpp"
#include "configmanager.hpp"

void layout_manager::Setup(browser::browser* handle) {
    handle->RegisterFunction("GetMainLayout", CREATE_REGISTRATION_MEMBER(layout_manager::PushLayout));

    m_frameMain = std::make_shared<ui::frame>("", ui::FL_HORIZONTAL, FRAME_TARGET_MAIN);
    m_frameMain->SetId("MainFrame");

    auto holderFrame = std::make_shared<ui::frame>("", ui::FL_VERTICAL);
    holderFrame->SetId("HolderFrame");
    m_frameMain->AddComponent<ui::frame>(holderFrame);

    auto lobbyFrame = std::make_shared<ui::frame>("lobby", ui::FL_VERTICAL);
    lobbyFrame->SetId("LobbyFrame");
    holderFrame->AddComponent<ui::frame>(std::move(lobbyFrame));

    auto autoPickerFrame = std::make_shared<ui::frame>("auto picker", ui::FL_VERTICAL);
    autoPickerFrame->SetId("AutoPickerFrame");
    holderFrame->AddComponent<ui::frame>(std::move(autoPickerFrame));

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