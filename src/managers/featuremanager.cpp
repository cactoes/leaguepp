#include "featuremanager.hpp"

#include "../interface/holder.hpp"
#include "layoutmanager.hpp"
#include "../features/lobbycontrolls.hpp"
#include "../features/autopicker.hpp"

void feature_manager::Setup() {
    auto frameMain = interface::GetHolder<layout_manager>(from_singleton)->GetFrame();
    auto targetFrame = frameMain->GetComponent<ui::frame>("HolderFrame");

    auto lobbyFrame = targetFrame->GetComponent<ui::frame>("LobbyFrame");
    auto autoPickerFrame = targetFrame->GetComponent<ui::frame>("AutoPickerFrame");

    CreateFeature<feature::lobby_controlls>(lobbyFrame);
    CreateFeature<feature::auto_picker>(autoPickerFrame);
}