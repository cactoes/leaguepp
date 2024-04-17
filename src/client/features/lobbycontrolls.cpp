#include "lobbycontrolls.hpp"

#include "../interface/holder.hpp"
#include "../managers/configmanager.hpp"
#include "../managers/connectormanager.hpp"

#undef interface

void feature::LobbyControlls::Setup(std::shared_ptr<ui::Frame> frame, IUiFramework* frameworkApiHandle) {
    auto connectorManager = interface<ConnectorManager>::Get();
    auto configManager = interface<ConfigManager>::Get();
    auto cfg = configManager->Get(CONFIG_BASIC);

    connectorManager->AddEventListener(
        "/lol-gameflow/v1/gameflow-phase",
        client_callback([this, cfg, connectorManager, frameworkApiHandle](std::string, nlohmann::json data) {
            auto currentGameFlow = data.get<std::string>();
            if (cfg->GetVar<bool>("lobby::bAutoAccept") && currentGameFlow == "ReadyCheck") {
                connector::result_t result = connectorManager->MakeRequest(connector::request_type::POST, "/lol-matchmaking/v1/ready-check/accept");
                if (result.status == 204)
                    frameworkApiHandle->CreateNotification("accepted match", "the match has been automatically accepted");
            }
        })
    );

    frame->AddCheckbox(
        "auto accept", cfg->GetVar<bool>("lobby::bAutoAccept"),
        ui::checkbox_callback([this, configManager, cfg](bool state) {
            return configManager->TrackedSetVar(cfg, "lobby::bAutoAccept", state);
        })
    );

    frame->AddButton(
        "dodge", ui::button_callback([this, connectorManager, frameworkApiHandle]() {
            auto result = connectorManager->MakeRequest(connector::request_type::GET, "/lol-gameflow/v1/gameflow-phase");
            if (result.status != 200 || result.data.get<std::string>() != "ChampSelect") {
                frameworkApiHandle->CreateNotification("failed to dodge", "not in a match");
                return;
            }

            // TODO: do we need to specify the data twice?
            result = connectorManager->MakeRequest(connector::request_type::POST,
                "/lol-login/v1/session/invoke?destination=lcdsServiceProxy&method=call&args=[\"\",\"teambuilder-draft\",\"quitV2\",\"\"]",
                "[\"\",\"teambuilder-draft\",\"quitV2\",\"\"]");

            if (result.status != 200)
                frameworkApiHandle->CreateNotification("failed to dodge", "the current match cannot be dodged");
            else
                frameworkApiHandle->CreateNotification("dodged", "the current match has been dodged");
        })
    );
}

std::string feature::LobbyControlls::GetName() {
    return "lobby controlls";
}