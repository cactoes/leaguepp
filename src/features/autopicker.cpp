#include "autopicker.hpp"

#include <ranges>

#include "../interface/holder.hpp"
#include "../managers/configmanager.hpp"
#include "../managers/connectormanager.hpp"
#include "../managers/browsermanager.hpp"

#include "../ui/selector.hpp"

void feature::auto_picker::Setup(std::shared_ptr<ui::frame> frame) {
    // auto connectorManager = interface::GetHolder<connector_manager>(from_singleton);
    auto configManager = interface::GetHolder<config_manager>(from_singleton);
    auto cfg = configManager->GetConfig(CONFIG_BASIC);

    frame->AddComponent<ui::selector>(
        "mode", cfg->GetVar<int>("autoPicker::nMode"), m_modes,
        ui::selector_callback_t([this, cfg, configManager](std::string newMode) {
            auto newModeIndex = (int)std::distance(m_modes.begin(), std::ranges::find(m_modes, newMode));
            cfg->SetVar("autoPicker::nMode", newModeIndex);
            configManager->DumpConfig(cfg);
            return newModeIndex;
        })
    );

    // frame->AddComponent<ui::checkbox>(
    //     GetName(), cfg->GetVar<bool>("lobby::bAutoAccept"),
    //     ui::checkbox_callback_t([this, configManager, cfg](bool state) {
    //         cfg->SetVar("lobby::bAutoAccept", state);
    //         configManager->DumpConfig(cfg);
    //         return state;
    //     })
    // );
}

std::string feature::auto_picker::GetName() {
    return "auto picker";
}
