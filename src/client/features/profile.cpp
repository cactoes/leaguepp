#include "profile.hpp"

#include <iostream>
#include <utils.hpp>

#include "../interface/holder.hpp"
#include "../managers/connectormanager.hpp"
#include "../managers/configmanager.hpp"
#include "../endpointmappers/summaryplayerdata.hpp"
#include "../endpointmappers/lolchatme.hpp"

#undef interface

void feature::Profile::Setup(std::shared_ptr<ui::Frame> frame, IUiFramework* frameworkApiHandle) {
    auto connectorManager = interface<ConnectorManager>::Get();
    auto configManager = interface<ConfigManager>::Get();
    auto cfg = configManager->Get(CONFIG_BASIC);

    auto label = frame->AddLabel("disconnected", "left");

    frame->AddButton(
        "clear tokens",
        ui::button_callback([this, connectorManager, frameworkApiHandle]() {
            auto playerDataResult = connectorManager->MakeRequest(connector::request_type::GET, "/lol-challenges/v1/summary-player-data/local-player");

            if (playerDataResult.status == 200) {
                auto data = playerDataResult.data.get<challenges::SummaryPlayerData>();

                const auto& bannerId = data.bannerId.value();
                const auto& titleId = data.title->itemId.value();

                auto result = connectorManager->MakeRequest(connector::request_type::POST, "/lol-challenges/v1/update-player-preferences",
                    "{\"bannerAccent\":\"" + bannerId + "\",\"challengeIds\":[],\"title\":\"" + std::to_string(titleId) + "\"}");

                if (result.status == 204) {
                    frameworkApiHandle->CreateNotification("cleared", "the tokens have been cleared");
                    return;
                }
            }

            frameworkApiHandle->CreateNotification("failed to clear", "the tokens could no be cleared");
        })
    );

    frame->AddDropdown(
        "tier", "", false, std::vector<std::string>{ cfg->GetVar<std::string>("profile::sTier") }, m_tiers,
        ui::dropdown_callback([this, configManager, cfg](std::string tier, bool, std::vector<std::string> list) {
            UpdateProfile([tier, configManager, cfg](lolchat::Me& me) {
                // RANKED_FLEX_SR
                me.lol->rankedLeagueQueue = "RANKED_SOLO_5x5";
                me.lol->rankedLeagueTier = tier;
                return true;
            });

            configManager->TrackedSetVar(cfg, "profile::sTier", tier);
            return list;
        })
    );

    frame->AddDropdown(
        "division", "", false, std::vector<std::string>{ cfg->GetVar<std::string>("profile::sDivision") }, m_divisions,
        ui::dropdown_callback([this, configManager, cfg](std::string division, bool, std::vector<std::string> list) {
            UpdateProfile([division, configManager, cfg](lolchat::Me& me) {
                me.lol->rankedLeagueDivision = division;
                return true;
            });

            configManager->TrackedSetVar(cfg, "profile::sDivision", division);
            return list;
        })
    );

    frame->AddInput("mastery score", "", cfg->GetVar<std::string>("profile::sMastery"), ui::input_callback([this, configManager, cfg](std::string input) {
        uint64_t masteryScore = 0;
        ParseNumber(input, masteryScore);

        auto mastery = std::to_string(masteryScore);

        UpdateProfile([mastery](lolchat::Me& me) {
            me.lol->masteryScore = mastery;
            return true;
        });

        return configManager->TrackedSetVar(cfg, "profile::sMastery", mastery);
    }));

    connectorManager->AddConnectHandler(
        client_connect([this, label, frameworkApiHandle]() {
            label->SetText("connected");
            frameworkApiHandle->CreateNotification("connected", "the client has connected to league");
        })
    );

    connectorManager->AddDisconnectHandler(
        client_disconnect([this, label, frameworkApiHandle]() {
            label->SetText("disconnected");
            frameworkApiHandle->CreateNotification("disconnected", "the client has disconnected from league");
        })
    );
}

std::string feature::Profile::GetName() {
    return "profile";
}

bool feature::Profile::IsValidItem(const std::vector<std::string>& list, const std::string& value) const {
    return std::ranges::any_of(list, [&](const auto& m_value) {
        return m_value == value;
    });
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
