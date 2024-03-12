#include "featuremanager.hpp"

#include "../interface/holder.hpp"
#include "layoutmanager.hpp"
#include "../features/autoAccept.hpp"

void feature_manager::Setup() {
    auto frameMain = interface::GetHolder<layout_manager>(from_singleton)->GetFrame();
    auto lobbyFrame = frameMain->GetComponent<ui::frame>("LobbyFrame");

    CreateFeature<feature::auto_accept>(lobbyFrame);
}