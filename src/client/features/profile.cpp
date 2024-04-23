#include "profile.hpp"

#include <iostream>
#include <utils.hpp>

#include "../interface/holder.hpp"
#include "../managers/connectormanager.hpp"
#include "../managers/configmanager.hpp"
#include "../endpointmappers/summaryplayerdata.hpp"
#include "../endpointmappers/lolchatme.hpp"

#include "../clientinterfacer.hpp"

#undef interface

void feature::Profile::Setup(std::shared_ptr<ui::Frame> frame, IUiFramework* frameworkApiHandle) {
    m_frameworkApiHandle = frameworkApiHandle;

    auto cfg = interface<ConfigManager>::Get()->Get(CONFIG_BASIC);
    m_cfg.profileTier = CVarHandle<std::string>(cfg, "profile::sTier");
    m_cfg.profileDivision = CVarHandle<std::string>(cfg, "profile::sDivision");
    m_cfg.profileMastery = CVarHandle<std::string>(cfg, "profile::sMastery");
    m_cfg.profileAutoSet = CVarHandle<bool>(cfg, "profile::bAutoSet");

    frame->AddCheckbox("auto update profile", NO_HINT, m_cfg.profileAutoSet.Get(),
        ui::checkbox_callback(&Profile::OnSetAutoUpdateProfile, this));

    frame->AddDropdown("tier", NO_HINT, SINGLE, std::vector<std::string>{ m_cfg.profileTier.Get() }, clientinterfacer::tiers,
        ui::dropdown_callback(&Profile::OnTierUpdate, this));

    frame->AddDropdown("division", NO_HINT, SINGLE, std::vector<std::string>{ m_cfg.profileDivision.Get() }, clientinterfacer::divisions,
        ui::dropdown_callback(&Profile::OnDivisionUpdate, this));

    frame->AddInput("mastery score", NO_HINT, m_cfg.profileMastery.Get(),
        ui::input_callback(&Profile::OnMasteryScoreInput, this));

    frame->AddButton("update profile", ui::button_callback(&Profile::OnClickUpdateProfile, this));

    frame->AddButton("clear tokens", ui::button_callback(&Profile::OnClickClearTokens, this));

    // auto label = frame->AddLabel("disconnected", "left");

    auto connectorManager = interface<ConnectorManager>::Get();

    connectorManager->AddEventListener("/lol-chat/v1/me",
        client_callback([this](std::string, nlohmann::json) {
            // qq
            if (m_cfg.profileAutoSet.Get())
                OnClickUpdateProfile();
        })
    );

    // connectorManager->AddConnectHandler(
    //     client_connect([this, label, frameworkApiHandle]() {
    //         label->SetText("connected");
    //         frameworkApiHandle->CreateNotification("connected", "the client has connected to league");
    //     })
    // );

    // connectorManager->AddDisconnectHandler(
    //     client_disconnect([this, label, frameworkApiHandle]() {
    //         label->SetText("disconnected");
    //         frameworkApiHandle->CreateNotification("disconnected", "the client has disconnected from league");
    //     })
    // );
}

std::string feature::Profile::GetName() {
    return "profile";
}

bool feature::Profile::UpdateProfile(std::function<bool(lolchat::Me& me)> setter) {
    auto connectorManager = interface<ConnectorManager>::Get();

    auto result = connectorManager->MakeRequest(connector::request_type::GET, "/lol-chat/v1/me");

    if (result.status != 200)
        return false;

    auto data = result.data.get<lolchat::Me>();

    if (!setter(data))
        return false;

    auto updateResult = connectorManager->MakeRequest(connector::request_type::PUT, "/lol-chat/v1/me", nlohmann::json(data).dump());
    return updateResult.status == 201;
}

std::vector<std::string> feature::Profile::OnTierUpdate(std::string tier, bool, std::vector<std::string> list) {
    UpdateProfile([tier](lolchat::Me& me) {
        me.lol->rankedLeagueQueue = clientinterfacer::queues::rankedSolo;
        me.lol->rankedLeagueTier = tier;
        return true;
    });

    m_cfg.profileTier.Set(tier);
    return list;
}
std::vector<std::string> feature::Profile::OnDivisionUpdate(std::string division, bool, std::vector<std::string> list) {
    UpdateProfile([division](lolchat::Me& me) {
        me.lol->rankedLeagueQueue = clientinterfacer::queues::rankedSolo;
        me.lol->rankedLeagueDivision = division;
        return true;
    });

    m_cfg.profileDivision.Set(division);
    return list;
}

std::string feature::Profile::OnMasteryScoreInput(std::string input) {
    uint64_t masteryScore = 0;
    ParseNumber(input, masteryScore);

    auto mastery = std::to_string(masteryScore);

    UpdateProfile([mastery](lolchat::Me& me) {
        me.lol->masteryScore = mastery;
        return true;
    });

    return m_cfg.profileMastery.Set(mastery);
}

void feature::Profile::OnClickUpdateProfile() {
    auto result = UpdateProfile([this](lolchat::Me& me) {
        me.lol->rankedLeagueQueue = clientinterfacer::queues::rankedSolo;
        me.lol->rankedLeagueTier = m_cfg.profileTier.Get();
        me.lol->rankedLeagueDivision = m_cfg.profileDivision.Get();
        me.lol->masteryScore = m_cfg.profileMastery.Get();
        return true;
    });

    if (!result)
        m_frameworkApiHandle->CreateNotification("failed to update", "the profile could not be updated");
}

bool feature::Profile::OnSetAutoUpdateProfile(bool state) {
    return m_cfg.profileAutoSet.Set(state);
}

void feature::Profile::OnClickClearTokens() {
    auto playerData = CIGetRequest<challenges::SummaryPlayerData>("/lol-challenges/v1/summary-player-data/local-player");
    if (playerData.has_value()) {
        const auto& bannerId = playerData->bannerId.value();
        const auto& titleId = playerData->title->itemId.value();

        if (CIPostRequest("/lol-challenges/v1/update-player-preferences", "{\"bannerAccent\":\"" + bannerId + "\",\"challengeIds\":[],\"title\":\"" + std::to_string(titleId) + "\"}")) {
            m_frameworkApiHandle->CreateNotification("cleared", "the tokens have been cleared");
            return;
        }
    }

    m_frameworkApiHandle->CreateNotification("failed to clear", "the tokens could no be cleared");
}
