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
            m_lobby_state.isInChampSelect = data.get<std::string>() == "ChampSelect";
        })
    );

    // TODO: lane based bans & picks
    // TODO: change the selectors
    // TODO: add inputs for the bans & picks

    cfg->SetVar("autoPicker::banIds", std::vector<int>{ 22, 74, 17 });
    cfg->SetVar("autoPicker::pickIds", std::vector<int>{ 51 });

    connectorManager->AddEventListener("/lol-champ-select/v1/session", client_callback_t([this, cfg](std::string uri, nlohmann::json data) {
        const auto sessionData = data.get<champselect::Session>();
        UpdateLobbyState(sessionData, cfg->GetVar<int>("autoPicker::nStrictness"));
        RunAutoPicker(sessionData, cfg);
    }));

    frame->AddComponent<ui::selector>(
        "mode", cfg->GetVar<int>("autoPicker::nMode"), m_modes,
        ui::selector_callback_t([this, cfg, configManager](std::string newMode) {
            auto newModeIndex = (int)std::distance(m_modes.begin(), std::ranges::find(m_modes, newMode));
            configManager->TrackedSetVar(cfg, "autoPicker::nMode", newModeIndex);
            return newModeIndex;
        })
    );

    frame->AddComponent<ui::selector>(
        "strictness", cfg->GetVar<int>("autoPicker::nStrictness"), m_strictnesses,
        ui::selector_callback_t([this, cfg, configManager](std::string newStrictness) {
            auto newStrictnessIndex = (int)std::distance(m_strictnesses.begin(), std::ranges::find(m_strictnesses, newStrictness));
            configManager->TrackedSetVar(cfg, "autoPicker::nStrictness", newStrictnessIndex);
            return newStrictnessIndex;
        })
    );
}

std::string feature::auto_picker::GetName() {
    return "auto picker";
}

void feature::auto_picker::UpdateLobbyState(const champselect::Session& data, int strictness) {
    m_lobby_state.lockedChampions.clear();
    for (const auto& pair : data.actions.value()) {
        for (const auto& action : pair) {
            if (action.completed.value())
                m_lobby_state.lockedChampions.push_back(action.championId.value());
        }
    }

    auto connectorManager = interface::GetHolder<connector_manager>(from_singleton);
    const auto result = connectorManager->MakeRequest(connector::request_type::GET, "/lol-lobby/v2/lobby");

    m_lobby_state.playerState = GetPlayerState(data);
    m_lobby_state.laneState = GetLaneState(data, result.data.get<lobby::Lobby>());
    m_lobby_state.isLaneValid = ValidateLaneState(strictness);
}

void feature::auto_picker::RunAutoPicker(const champselect::Session& data, std::shared_ptr<config> cfg) {
    int mode = cfg->GetVar<int>("autoPicker::nMode");

    if (mode == BOT_MANUAL || !m_lobby_state.isInChampSelect || !m_lobby_state.isLaneValid)
        return;

    // TODO: notif the user with the banned / picked champ name

    switch (m_lobby_state.playerState) {
        case player_state::DECLARING:
            (void)MakeAction(data, action_type::PICK, false, cfg->GetVar<std::vector<int>>("autoPicker::pickIds"));
            break;
        case player_state::BANNING:
            (void)MakeAction(data, action_type::BAN, mode == BOT_AUTO, cfg->GetVar<std::vector<int>>("autoPicker::banIds"));
            break;
        case player_state::PICKING:
            (void)MakeAction(data, action_type::PICK, mode == BOT_AUTO, cfg->GetVar<std::vector<int>>("autoPicker::pickIds"));
            break;
        case player_state::INVALID:
        case player_state::WAITING:
        default:
            break;
    }
}

bool feature::auto_picker::MakeAction(const champselect::Session& data, action_type type, bool commit, const std::vector<int> options) {
    for (const auto& pair : data.actions.value()) {
        for (const auto& action : pair) {
            // if (action.completed.value())
            //     continue;

            if (action.actorCellId.value() != data.localPlayerCellId.value())
                continue;

            if ((type == action_type::PICK && action.type.value() != "pick") ||
                (type == action_type::BAN && action.type.value() != "ban"))
                continue;

            if (action.championId.value())
                continue;

            
            if (const int championId = GetNextPick(options); championId != -1) {
                return DoAction(static_cast<int>(action.id.value()), championId, commit);
            }
        }
    }
    
    return false;
}

int feature::auto_picker::GetNextPick(const std::vector<int>& list) {
    for (const auto& id : list)
        if (!std::ranges::count(m_lobby_state.lockedChampions, id) && id != -1)
            return id;

    return -1;
}

feature::player_state feature::auto_picker::GetPlayerState(const champselect::Session& data) {
    const auto currentPhase = data.timer->phase.value();

    if (currentPhase == "PLANNING")
        return player_state::DECLARING;

    if (currentPhase == "FINALIZATION")
        return player_state::WAITING;

    if (currentPhase == "BAN_PICK") {
        int64_t localPlayerCellId = data.localPlayerCellId.value();

        for (const auto& pair : data.actions.value()) {
            for (const auto& action : pair) {
                if (action.completed.value())
                    continue;

                if (action.actorCellId.value() != localPlayerCellId)
                    continue;

                if (action.isInProgress.value()) {
                    return action.type.value() == "ban" ?
                        player_state::BANNING :
                        player_state::PICKING;
                }

                return player_state::WAITING;
            }
        }
    }

    return player_state::INVALID;
}

// CLEAN: move
static bool StringCompareI(std::string_view a, std::string_view b) {
    return std::ranges::equal(a, b, [](char a, char b) {
        return std::tolower(static_cast<int>(a)) == std::tolower(static_cast<int>(b));
    });
}

feature::lane_state feature::auto_picker::GetLaneState(const champselect::Session& session, const lobby::Lobby& lobby) {
    if (!lobby.gameConfig.has_value())
        return lane_state::INVALID;

    const auto queueId = lobby.gameConfig->queueId.value();
    if (lobby.gameConfig->isCustom.value() || queueId == QT_INVALID)
        return lane_state::NO_PICKABLE_LANE;

    if (queueId != QT_SOLO_DUO && queueId != QT_FLEX && queueId != QT_DRAFT)
        return lane_state::NO_PICKABLE_LANE;

    auto myTeamIterator = std::ranges::find_if(session.myTeam.value(), [session] (const champselect::MyTeam& obj) {
        return obj.cellId.value() == session.localPlayerCellId.value();
    });

    if (myTeamIterator == session.myTeam->end())
        return lane_state::INVALID;

    std::string assignedPosition = myTeamIterator->assignedPosition.value();

    if (StringCompareI(assignedPosition, lobby.localMember->firstPositionPreference.value()))
        return lane_state::ASSIGNED_PRIMARY_POSITION;

    if (StringCompareI(assignedPosition, lobby.localMember->secondPositionPreference.value()))
        return lane_state::ASSIGNED_SECONDARY_POSITION;

    return lane_state::INVALID;
}

bool feature::auto_picker::ValidateLaneState(int strictness) {
    // strictness 0 means we dont care
    // about what lane we are in
    if (strictness == 0)
        return true;

    // the lane aint there yo
    if (m_lobby_state.laneState == lane_state::INVALID)
        return false;

    // strictness 1 means we only want to run
    // if we have our secondary or primary lane
    if (strictness == 1) {
        // in old logic we checked if lanestate was not "NoPickableLane"
        // but this seems better
        return m_lobby_state.laneState == lane_state::ASSIGNED_PRIMARY_POSITION ||
            m_lobby_state.laneState == lane_state::ASSIGNED_SECONDARY_POSITION;
    }

    // here we have strictness 2
    // wich mean we only want to run when we have
    // our primary lane
    return m_lobby_state.laneState == lane_state::ASSIGNED_PRIMARY_POSITION;
}

bool feature::auto_picker::DoAction(int actionId, int championId, bool commit) {
    auto connectionManager = interface::GetHolder<connector_manager>(from_singleton);

    auto declareResult = connectionManager->MakeRequest(
        connector::request_type::PATCH,
        std::format("/lol-champ-select/v1/session/actions/{}", actionId),
        std::format("{}\"championId\":{}{}", "{", championId, "}")
    );

    if (declareResult.status != 204)
        return false;

    if (commit) {
        auto commitResult = connectionManager->MakeRequest(
            connector::request_type::POST,
            std::format("/lol-champ-select/v1/session/actions/{}/complete", actionId),
            std::format("{}\"championId\":{}{}", "{", championId, "}")
        );

        if (commitResult.status != 204)
            return false;
    }

    return true;
}