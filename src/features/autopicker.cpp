#include "autopicker.hpp"

#include <ranges>

#include "../interface/holder.hpp"
#include "../managers/configmanager.hpp"
#include "../managers/connectormanager.hpp"
#include "../managers/browsermanager.hpp"

#include "../ui/selector.hpp"
#include "../ui/button.hpp"
#include "../ui/label.hpp"

void feature::auto_picker::Setup(std::shared_ptr<ui::frame> frame) {
    auto connectorManager = interface::GetHolder<connector_manager>(from_singleton);
    auto configManager = interface::GetHolder<config_manager>(from_singleton);
    auto cfg = configManager->GetConfig(CONFIG_BASIC);

    connectorManager->AddEventListener(
        "/lol-gameflow/v1/gameflow-phase",
        client_callback_t([this, cfg, connectorManager](nlohmann::json data) {
            if (auto currentGameFlow = data.get<std::string>(); currentGameFlow != "ChampSelect")
                return;

            // TODO: handlers for champselect suff
        })
    );

    frame->AddComponent<ui::selector>(
        "mode", cfg->GetVar<int>("autoPicker::nMode"), m_modes,
        ui::selector_callback_t([this, cfg, configManager](std::string newMode) {
            auto newModeIndex = (int)std::distance(m_modes.begin(), std::ranges::find(m_modes, newMode));
            cfg->SetVar("autoPicker::nMode", newModeIndex);
            configManager->DumpConfig(cfg);
            return newModeIndex;
        })
    );

    frame->AddComponent<ui::button>(
        "dodge", ui::button_callback_t([this, connectorManager]() {
            // TODO: create notification to let user know if it failed

            auto result = connectorManager->MakeRequest(connector::request_type::GET, "/lol-gameflow/v1/gameflow-phases");
            if (result.status != 200 || result.data.get<std::string>() != "ChampSelect")
                return;

            // TODO: do we need to specify the data twice?
            (void)connectorManager->MakeRequest(connector::request_type::POST,
                "/lol-login/v1/session/invoke?destination=lcdsServiceProxy&method=call&args=[\"\",\"teambuilder-draft\",\"quitV2\",\"\"]",
                "[\"\",\"teambuilder-draft\",\"quitV2\",\"\"]");
        })
    );
}

std::string feature::auto_picker::GetName() {
    return "auto picker";
}
