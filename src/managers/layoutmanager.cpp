#include "layoutmanager.hpp"

#include "../interface/holder.hpp"
#include "../ui/frame.hpp"
#include "../browser/browser.hpp"

#include "browsermanager.hpp"

#undef interface

bool LayoutManager::Init() {
    auto browserManager = interface<BrowserManager>::Get();
    browserManager->GetHandle()->RegisterFunction("GetMainLayout", CREATE_REGISTRATION_MEMBER(LayoutManager::PushLayout));

    m_frameMain = std::make_shared<ui::Frame>("", ui::FL_HORIZONTAL, FRAME_TARGET_MAIN);
    m_frameMain->SetId("MainFrame");

    auto holderFrame = std::make_shared<ui::Frame>("", ui::FL_VERTICAL);
    holderFrame->SetId("HolderFrame");
    m_frameMain->AddComponent<ui::Frame>(holderFrame);

    auto lobbyFrame = std::make_shared<ui::Frame>("lobby", ui::FL_VERTICAL_AUTO);
    lobbyFrame->SetId("LobbyFrame");
    holderFrame->AddComponent<ui::Frame>(std::move(lobbyFrame));

    auto autoPickerFrame = std::make_shared<ui::Frame>("auto picker", ui::FL_VERTICAL_AUTO);
    autoPickerFrame->SetId("AutoPickerFrame");
    holderFrame->AddComponent<ui::Frame>(std::move(autoPickerFrame));

    auto profileFrame = std::make_shared<ui::Frame>("profile", ui::FL_VERTICAL);
    profileFrame->SetId("ProfileFrame");
    m_frameMain->AddComponent<ui::Frame>(std::move(profileFrame));

    return true;
}

void LayoutManager::PushLayout(browser::Browser* handle, browser::js_args_t args) {
    m_frameMain->Register(handle);
}

std::shared_ptr<ui::Frame> LayoutManager::GetFrame() {
    return m_frameMain;
}