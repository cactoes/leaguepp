#include "lobbycontrolls.hpp"

#include "../interface/holder.hpp"
#include "../managers/configmanager.hpp"
#include "../managers/connectormanager.hpp"

#include "../endpointmappers/champselectsession.hpp"
#include "../clientinterfacer.hpp"

#include <utils.hpp>

#undef interface

void feature::LobbyControlls::Setup(std::shared_ptr<ui::Frame> frame, IUiFramework* frameworkApiHandle) {
    m_frameworkApiHandle = frameworkApiHandle;

    auto cfg = interface<ConfigManager>::Get()->Get(CONFIG_BASIC);
    m_cfg.lobbyAutoAccept = CVarHandle<bool>(cfg, "lobby::bAutoAccept");
    m_cfg.lobbyAutoDodge = CVarHandle<bool>(cfg, "lobby::bAutoDodge");
    m_cfg.lobbyAutoDodgeLanes = CVarHandle<std::vector<int>>(cfg, "lobby::autoDodgeLanes");

    frame->AddCheckbox("auto accept", NO_HINT, m_cfg.lobbyAutoAccept.Get(),
        ui::checkbox_callback(&LobbyControlls::OnSetAutoAccept, this));

    frame->AddCheckbox("auto dodge", NO_HINT, m_cfg.lobbyAutoDodge.Get(),
        ui::checkbox_callback(&LobbyControlls::OnSetAutoDodge, this));

    frame->AddDropdown("auto dodge lanes", NO_HINT, MULTI,
        ConvertAToB<std::string, int>(m_cfg.lobbyAutoDodgeLanes.Get(), clientinterfacer::IdsToLanesConvertor), clientinterfacer::lanes,
        ui::dropdown_callback(&LobbyControlls::OnAutoDodgeLanesUpdate, this));

    frame->AddButton("dodge", ui::button_callback(&LobbyControlls::OnClickDodge, this));

    auto connectorManager = interface<ConnectorManager>::Get();
    connectorManager->AddEventListener("/lol-gameflow/v1/gameflow-phase",
        client_callback([this](std::string, nlohmann::json data) {
            OnNewGameflow(data.get<std::string>());
        })
    );
}

std::string feature::LobbyControlls::GetName() {
    return "lobby controlls";
}

void feature::LobbyControlls::OnNewGameflow(const std::string& gameFlow) {
    if (gameFlow == "ChampSelect" && m_cfg.lobbyAutoDodge.Get()) {
        const auto session = CIGetRequest<champselect::Session>("/lol-champ-select/v1/session").value();
        const auto lanes = ConvertAToB<std::string, int>(m_cfg.lobbyAutoDodgeLanes.Get(), clientinterfacer::IdsToLanesConvertor);
        if (auto it = std::ranges::find(lanes, CIGetAssignedPosition(session)); it == lanes.end())
            (void)CIDodgeQueue();
    }

    if (gameFlow == "ReadyCheck" && m_cfg.lobbyAutoAccept.Get())
        if (CIPostRequest("/lol-matchmaking/v1/ready-check/accept"))
            m_frameworkApiHandle->CreateNotification("accepted match", "the match has been automatically accepted");
}

bool feature::LobbyControlls::OnSetAutoAccept(bool state) {
    return m_cfg.lobbyAutoAccept.Set(state);
}

void feature::LobbyControlls::OnClickDodge() {
    const auto phase = CIGetRequest<std::string>("/lol-gameflow/v1/gameflow-phase");
    if (phase.has_value() && phase.value() != "ChampSelect") {
        m_frameworkApiHandle->CreateNotification("failed to dodge", "not in a match");
        return;
    }

    const auto hasDodged = CIDodgeQueue();
    m_frameworkApiHandle->CreateNotification(
        hasDodged ? "dodged" : "failed to dodge",
        hasDodged ? "the current match has been dodged" : "the current match cannot be dodged"
    );
}

bool feature::LobbyControlls::OnSetAutoDodge(bool state) {
    return m_cfg.lobbyAutoDodge.Set(state);
}

std::vector<std::string> feature::LobbyControlls::OnAutoDodgeLanesUpdate(std::string, bool, std::vector<std::string> list) {
    m_cfg.lobbyAutoDodgeLanes.Set(ConvertAToB<int, std::string>(list, clientinterfacer::LanesToIdsConvertor));
    return list;
}