#include "autoAccept.hpp"

#include "../interface/holder.hpp"
#include "../managers/configmanager.hpp"
#include "../managers/connectormanager.hpp"
#include "../managers/browsermanager.hpp"

void feature::auto_accept::Setup(std::shared_ptr<ui::frame> frame) {
    auto connectorManager = interface::GetHolder<connector_manager>(from_singleton);
    auto configManager = interface::GetHolder<config_manager>(from_singleton);
    auto cfg = configManager->GetConfig(CONFIG_BASIC);

    connectorManager->AddEventListener(
        "/lol-gameflow/v1/gameflow-phase",
        client_callback_t([this, cfg, connectorManager](nlohmann::json data) {
            auto currentGameFlow = data.get<std::string>();
            if (cfg->GetVar<bool>("lobby::bAutoAccept") && currentGameFlow == "ReadyCheck") {
                (void)connectorManager->MakeRequest(connector::request_type::POST, "/lol-matchmaking/v1/ready-check/accept");
                // connector::result_t result;
                // if (result.status == 200)
                // interface::GetHolder<browser_manager>(from_singleton)->CallJS("createNotification", { "Notification title", 10 });
            }
        })
    );

    frame->AddComponent<ui::checkbox>(
        GetName(), cfg->GetVar<bool>("lobby::bAutoAccept"),
        ui::checkbox_callback_t([this, configManager, cfg](bool state) {
            cfg->SetVar("lobby::bAutoAccept", state);
            configManager->DumpConfig(cfg);
            return state;
        })
    );
}

std::string feature::auto_accept::GetName() {
    return "auto accept";
}