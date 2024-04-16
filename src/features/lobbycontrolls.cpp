#include "lobbycontrolls.hpp"

#include "../interface/holder.hpp"
#include "../managers/configmanager.hpp"
#include "../managers/connectormanager.hpp"
#include "../managers/browsermanager.hpp"

#include "../ui/checkbox.hpp"
#include "../ui/button.hpp"
#include "../ui/frame.hpp"

#undef interface

void feature::LobbyControlls::Setup(std::shared_ptr<ui::Frame> frame) {
    auto connectorManager = interface<ConnectorManager>::Get();
    auto configManager = interface<ConfigManager>::Get();
    auto cfg = configManager->Get(CONFIG_BASIC);

    connectorManager->AddEventListener(
        "/lol-gameflow/v1/gameflow-phase",
        client_callback([this, cfg, connectorManager](std::string, nlohmann::json data) {
            auto currentGameFlow = data.get<std::string>();
            if (cfg->GetVar<bool>("lobby::bAutoAccept") && currentGameFlow == "ReadyCheck") {
                connector::result_t result = connectorManager->MakeRequest(connector::request_type::POST, "/lol-matchmaking/v1/ready-check/accept");
                if (result.status == 204)
                    interface<BrowserManager>::Get()->CreateNotification("accepted match", "the match has been automatically accepted", notification_type::SUCCESS);
            }
        })
    );

    frame->AddComponent<ui::Checkbox>(
        "auto accept", cfg->GetVar<bool>("lobby::bAutoAccept"),
        ui::checkbox_callback([this, configManager, cfg](bool state) {
            return configManager->TrackedSetVar(cfg, "lobby::bAutoAccept", state);
        })
    );

    frame->AddComponent<ui::Button>(
        "dodge", ui::button_callback([this, connectorManager]() {
            auto browserManager = interface<BrowserManager>::Get();

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

std::string feature::LobbyControlls::GetName() {
    return "lobby controlls";
}