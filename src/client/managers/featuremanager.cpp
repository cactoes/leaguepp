#include "featuremanager.hpp"

#include "../features/lobbycontrolls.hpp"
#include "../features/autopicker.hpp"
#include "../features/profile.hpp"

bool FeatureManager::Init(IUiFramework* frameworkApiHandle) {
    auto miscTab = frameworkApiHandle->AddTab("misc", true);
    auto frameMain = miscTab->GetFrame();

    auto autoPickerFrame = frameMain->AddFrame("auto picker", ui::FL_VERTICAL_AUTO);

    auto targetFrame = frameMain->AddFrame("", ui::FL_VERTICAL);
    auto lobbyFrame = targetFrame->AddFrame("lobby", ui::FL_VERTICAL_AUTO);
    auto profileFrame = targetFrame->AddFrame("profile", ui::FL_VERTICAL);

    CreateFeature<feature::LobbyControlls>(lobbyFrame, frameworkApiHandle);
    CreateFeature<feature::AutoPicker>(autoPickerFrame, frameworkApiHandle);
    CreateFeature<feature::Profile>(profileFrame, frameworkApiHandle);

    // frameworkApiHandle->AddTab("lobby");

    return true;
}