#include "autoAccept.hpp"

#include "../interface/holder.hpp"
#include "../managers/configmanager.hpp"

void feature::auto_accept::Setup(std::shared_ptr<ui::frame> frame) {
    auto configManager = interface::GetHolder<config_manager>(from_singleton);
    auto cfg = configManager->GetConfig("cfg");

    frame->AddComponent<ui::checkbox>(
        GetName(), cfg->GetVar<bool>("lobby::autoAccept"),
        ui::checkbox_callback_t([this, configManager, cfg] (bool state) {
            cfg->SetVar("lobby::autoAccept", state);
            configManager->DumpConfig(cfg);
            return state;
        })
    );
}

std::string feature::auto_accept::GetName() {
    return "auto accept";
}