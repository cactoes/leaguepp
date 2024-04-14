#include "lobbycontrolls.hpp"

#include "../interface/holder.hpp"
#include "../managers/configmanager.hpp"
#include "../managers/connectormanager.hpp"
#include "../managers/browsermanager.hpp"

#include "../ui/checkbox.hpp"
#include "../ui/button.hpp"

void feature::lobby_controlls::Setup(std::shared_ptr<ui::frame> frame) {
    auto connectorManager = interface::GetHolder<connector_manager>(from_singleton);
    auto configManager = interface::GetHolder<config_manager>(from_singleton);
    auto cfg = configManager->GetConfig(CONFIG_BASIC);

    connectorManager->AddEventListener(
        "/lol-gameflow/v1/gameflow-phase",
        client_callback_t([this, cfg, connectorManager](std::string, nlohmann::json data) {
            auto currentGameFlow = data.get<std::string>();
            if (cfg->GetVar<bool>("lobby::bAutoAccept") && currentGameFlow == "ReadyCheck") {
                connector::result_t result = connectorManager->MakeRequest(connector::request_type::POST, "/lol-matchmaking/v1/ready-check/accept");
                if (result.status == 204)
                    interface::GetHolder<browser_manager>(from_singleton)->CreateNotification("accepted match", "the match has been automatically accepted", notification_type::SUCCESS);
            }
        })
    );

    frame->AddComponent<ui::checkbox>(
        "auto accept", cfg->GetVar<bool>("lobby::bAutoAccept"),
        ui::checkbox_callback_t([this, configManager, cfg](bool state) {
            cfg->SetVar("lobby::bAutoAccept", state);
            configManager->DumpConfig(cfg);
            return state;
        })
    );

    frame->AddComponent<ui::button>(
        "dodge", ui::button_callback_t([this, connectorManager]() {
            auto browserManager = interface::GetHolder<browser_manager>(from_singleton);

            auto result = connectorManager->MakeRequest(connector::request_type::GET, "/lol-gameflow/v1/gameflow-phase");
            if (result.status != 200 || result.data.get<std::string>() != "ChampSelect") {
                browserManager->CreateNotification("failed to dodge", "not in a match", notification_type::NONE);
                return;
            }

            // TODO: do we need to specify the data twice?
            result = connectorManager->MakeRequest(connector::request_type::POST,
                "/lol-login/v1/session/invoke?destination=lcdsServiceProxy&method=call&args=[\"\",\"teambuilder-draft\",\"quitV2\",\"\"]",
                "[\"\",\"teambuilder-draft\",\"quitV2\",\"\"]");

            if (result.status != 200)
                browserManager->CreateNotification("failed to dodge", "the current match cannot be dodged", notification_type::NONE);
            else
                browserManager->CreateNotification("dodged", "the current match has been dodged", notification_type::NONE);
        })
    );
}

std::string feature::lobby_controlls::GetName() {
    return "lobby controlls";
}