#include "featuremanager.hpp"

#include "../features/lobbycontrolls.hpp"
#include "../features/autopicker.hpp"
#include "../features/profile.hpp"

bool FeatureManager::Init(IUiFramework* frameworkApiHandle) {
    auto frameMain = frameworkApiHandle->GetMainFrame();
    auto targetFrame = frameMain->AddFrame("", ui::FL_VERTICAL);

    auto lobbyFrame = targetFrame->AddFrame("lobby", ui::FL_VERTICAL_AUTO);
    auto autoPickerFrame = targetFrame->AddFrame("auto picker", ui::FL_VERTICAL_AUTO);
    auto profileFrame = frameMain->AddFrame("profile", ui::FL_VERTICAL_AUTO);

    CreateFeature<feature::LobbyControlls>(lobbyFrame, frameworkApiHandle);
    CreateFeature<feature::AutoPicker>(autoPickerFrame, frameworkApiHandle);
    CreateFeature<feature::Profile>(profileFrame, frameworkApiHandle);

    return true;
}