#include "autopicker.hpp"

#include <ranges>

#include "../interface/holder.hpp"
#include "../managers/configmanager.hpp"
#include "../managers/connectormanager.hpp"
#include "../managers/browsermanager.hpp"

#include "../ui/selector.hpp"
#include "../ui/button.hpp"
#include "../ui/label.hpp"

#include "../debugutils.hpp"

void feature::auto_picker::Setup(std::shared_ptr<ui::frame> frame) {
    auto connectorManager = interface::GetHolder<connector_manager>(from_singleton);
    auto configManager = interface::GetHolder<config_manager>(from_singleton);
    auto cfg = configManager->GetConfig(CONFIG_BASIC);

    connectorManager->AddEventListener(
        "/lol-gameflow/v1/gameflow-phase",
        client_callback_t([this, cfg, connectorManager](std::string, nlohmann::json data) {
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
}

std::string feature::auto_picker::GetName() {
    return "auto picker";
}
