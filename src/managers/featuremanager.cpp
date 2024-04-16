#include "featuremanager.hpp"

#include "../interface/holder.hpp"
#include "layoutmanager.hpp"
#include "../features/lobbycontrolls.hpp"
#include "../features/autopicker.hpp"
#include "../ui/frame.hpp"

#undef interface

bool FeatureManager::Init() {
    auto frameMain = interface<LayoutManager>::Get()->GetFrame();
    auto targetFrame = frameMain->GetComponent<ui::Frame>("HolderFrame");

    auto lobbyFrame = targetFrame->GetComponent<ui::Frame>("LobbyFrame");
    auto autoPickerFrame = targetFrame->GetComponent<ui::Frame>("AutoPickerFrame");

    CreateFeature<feature::LobbyControlls>(lobbyFrame);
    CreateFeature<feature::AutoPicker>(autoPickerFrame);

    return true;
}