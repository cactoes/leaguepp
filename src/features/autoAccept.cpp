#include "autoAccept.hpp"

#include <iostream>

#include "../interface/holder.hpp"
#include "../managers/configmanager.hpp"
#include "../managers/connectormanager.hpp"

void feature::auto_accept::Setup(std::shared_ptr<ui::frame> frame) {
    auto configManager = interface::GetHolder<config_manager>(from_singleton);
    auto cfg = configManager->GetConfig("cfg");

    auto connectorManager = interface::GetHolder<connector_manager>(from_singleton);
    connectorManager->AddEventListener(
        "/lol-gameflow/v1/gameflow-phase",
        client_callback_t([this, cfg, connectorManager](nlohmann::json data) {
            auto currentGameFlow = data.get<std::string>();
            if (cfg->GetVar<bool>("lobby::autoAccept") && currentGameFlow == "ReadyCheck")
                (void)connectorManager->MakeRequest(connector::request_type::POST, "/lol-matchmaking/v1/ready-check/accept");
        })
    );

    frame->AddComponent<ui::checkbox>(
        GetName(), cfg->GetVar<bool>("lobby::autoAccept"),
        ui::checkbox_callback_t([this, configManager, cfg](bool state) {
            cfg->SetVar("lobby::autoAccept", state);
            configManager->DumpConfig(cfg);
            return state;
        })
    );
}

std::string feature::auto_accept::GetName() {
    return "auto accept";
}