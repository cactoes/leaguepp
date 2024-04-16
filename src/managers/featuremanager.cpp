#include "featuremanager.hpp"

#include "../interface/holder.hpp"
#include "../ui/frame.hpp"

#include "../features/lobbycontrolls.hpp"
#include "../features/autopicker.hpp"
#include "../features/stats.hpp"

#include "layoutmanager.hpp"

#undef interface

bool FeatureManager::Init() {
    auto frameMain = interface<LayoutManager>::Get()->GetFrame();
    auto targetFrame = frameMain->GetComponent<ui::Frame>("HolderFrame");

    auto lobbyFrame = targetFrame->GetComponent<ui::Frame>("LobbyFrame");
    auto autoPickerFrame = targetFrame->GetComponent<ui::Frame>("AutoPickerFrame");
    auto statsFrame = frameMain->GetComponent<ui::Frame>("StatsFrame");

    CreateFeature<feature::LobbyControlls>(lobbyFrame);
    CreateFeature<feature::AutoPicker>(autoPickerFrame);
    CreateFeature<feature::Stats>(statsFrame);

    return true;
}