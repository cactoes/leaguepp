#include "autopicker.hpp"

#include "../interface/holder.hpp"
#include "../managers/configmanager.hpp"
#include "../managers/connectormanager.hpp"
#include "../managers/browsermanager.hpp"

#include "../ui/selector.hpp"
#include "../ui/button.hpp"
#include "../ui/label.hpp"

#include "../utils.hpp"

void feature::auto_picker::Setup(std::shared_ptr<ui::frame> frame) {
    auto connectorManager = interface<connector_manager>::Get();
    m_config = interface<config_manager>::Get()->GetConfig(CONFIG_BASIC);

    // TODO: lane based bans & picks
    // TODO: change the selectors
    // TODO: add inputs for the bans & picks

    m_config->SetVar("autoPicker::banIds", std::vector<int>{ 22, 74, 17 });
    m_config->SetVar("autoPicker::pickIds", std::vector<int>{ 51 });

    // setup hooks

    connectorManager->AddEventListener(
        "/lol-gameflow/v1/gameflow-phase",
        client_callback_t([this](std::string, nlohmann::json data) {
            m_lobby_info.isInChampSelect = data.get<std::string>() == "ChampSelect";
        })
    );

    connectorManager->AddEventListener("/lol-champ-select/v1/session", client_callback_t([this, connectorManager](std::string, nlohmann::json data) {
        const auto lobbyDataResult = connectorManager->MakeRequest(connector::request_type::GET, "/lol-lobby/v2/lobby");
        if (lobbyDataResult.status != 200) {
            NotifyUser("error", "failed to get lobby data");
            return;
        }

        HandleFrame(data.get<champselect::Session>(), lobbyDataResult.data.get<lobby::Lobby>());
    }));

    // setup ui

    frame->AddComponent<ui::selector>(
        "mode", m_config->GetVar<int>("autoPicker::nMode"), m_modes,
        ui::selector_callback_t([this](std::string newMode) {
            const auto newModeIndex = (int)std::distance(m_modes.begin(), std::ranges::find(m_modes, newMode));
            interface<config_manager>::Get()->TrackedSetVar(m_config, "autoPicker::nMode", newModeIndex);
            return newModeIndex;
        })
    );

    frame->AddComponent<ui::selector>(
        "strictness", m_config->GetVar<int>("autoPicker::nStrictness"), m_strictnesses,
        ui::selector_callback_t([this](std::string newStrictness) {
            const auto newStrictnessIndex = (int)std::distance(m_strictnesses.begin(), std::ranges::find(m_strictnesses, newStrictness));
            interface<config_manager>::Get()->TrackedSetVar(m_config, "autoPicker::nStrictness", newStrictnessIndex);
            return newStrictnessIndex;
        })
    );
}

std::string feature::auto_picker::GetName() {
    return "auto picker";
}

std::vector<int> feature::auto_picker::GetLockedChampions(const champselect::Session& session) {
    std::vector<int> lockedChampions = {};
    for (const auto& pair : session.actions.value())
        for (const auto& action : pair)
            if (action.completed.value())
                lockedChampions.push_back(static_cast<int>(action.championId.value()));

    return lockedChampions;
}

feature::player_state feature::auto_picker::GetPlayerState(const champselect::Session& session) {
    const auto& currentPhase = session.timer->phase.value();

    if (currentPhase == "PLANNING")
        return player_state::DECLARING;

    if (currentPhase == "FINALIZATION")
        return player_state::WAITING;

    if (currentPhase != "BAN_PICK")
        return player_state::INVALID;

    const auto& localPlayerCellId = session.localPlayerCellId.value();

    for (const auto& pair : session.actions.value()) {
        for (const auto& action : pair) {
            if (action.completed.value())
                continue;

            if (action.actorCellId.value() != localPlayerCellId)
                continue;

            if (action.isInProgress.value())
                return action.type.value() == "ban" ? player_state::BANNING : player_state::PICKING;

            return player_state::WAITING;
        }
    }

    // we shouldn't get here tho
    return player_state::INVALID;
}

feature::lane_state feature::auto_picker::GetLaneState(const champselect::Session& session, const lobby::Lobby& lobby) {
    // verify the game config
    if (!lobby.gameConfig.has_value())
        return lane_state::INVALID;

    // verify the game type
    const auto& queueId = lobby.gameConfig->queueId.value();
    if (lobby.gameConfig->isCustom.value() || queueId == QT_INVALID)
        return lane_state::NO_PICKABLE_LANE;

    if (queueId != QT_SOLO_DUO && queueId != QT_FLEX && queueId != QT_DRAFT)
        return lane_state::NO_PICKABLE_LANE;

    // get our assigned position
    auto myTeamIterator = std::ranges::find_if(session.myTeam.value(), [session](const champselect::MyTeam& obj) {
        return obj.cellId.value() == session.localPlayerCellId.value();
    });

    if (myTeamIterator == session.myTeam->end())
        return lane_state::INVALID;

    std::string assignedPosition = myTeamIterator->assignedPosition.value();

    // do lane based checks
    if (StringCompareI(assignedPosition, lobby.localMember->firstPositionPreference.value()))
        return lane_state::ASSIGNED_PRIMARY_POSITION;

    if (StringCompareI(assignedPosition, lobby.localMember->secondPositionPreference.value()))
        return lane_state::ASSIGNED_SECONDARY_POSITION;

    return lane_state::OTHER_POSITION;
}

bool feature::auto_picker::ValidateLaneState(lane_state state, int strictness) {
    // strictness 0 means we dont care
    // about what lane we are in
    if (strictness == 0)
        return true;

    // the lane aint there yo
    if (state == lane_state::INVALID)
        return false;

    // strictness 1 means we only want to run
    // if we have our secondary or primary lane
    if (strictness == 1) {
        // in old logic we checked if lanestate was not "NoPickableLane"
        // but this seems better
        return state == lane_state::ASSIGNED_PRIMARY_POSITION ||
            state == lane_state::ASSIGNED_SECONDARY_POSITION;
    }

    // here we have strictness 2
    // wich mean we only want to run when we have
    // our primary lane
    return state == lane_state::ASSIGNED_PRIMARY_POSITION;
}

void feature::auto_picker::HandleFrame(const champselect::Session& session, const lobby::Lobby& lobby) {
    const int mode = m_config->GetVar<int>("autoPicker::nMode");
    if (mode == BOT_MANUAL || !m_lobby_info.isInChampSelect)
        return;

    const int strictness = m_config->GetVar<int>("autoPicker::nStrictness");
    const auto laneState = GetLaneState(session, lobby);
    if (!ValidateLaneState(laneState, strictness))
        return;

    m_lobby_info.lockedChampions = GetLockedChampions(session);

    // TODO: notif the user with the banned / picked champ name
    // NotifyUser("declared champion", "champion_name");

    switch (GetPlayerState(session)) {
        case player_state::DECLARING:
            (void)MakeAction(session, action_type::PICK, m_config->GetVar<std::vector<int>>("autoPicker::pickIds"), false);
            break;
        case player_state::BANNING:
            (void)MakeAction(session, action_type::BAN, m_config->GetVar<std::vector<int>>("autoPicker::banIds"), mode == BOT_AUTO);
            break;
        case player_state::PICKING:
            (void)MakeAction(session, action_type::PICK, m_config->GetVar<std::vector<int>>("autoPicker::pickIds"), mode == BOT_AUTO);
            break;
        case player_state::INVALID:
        case player_state::WAITING:
        default:
            break;
    }
}

int feature::auto_picker::MakeAction(const champselect::Session& session, action_type type, const std::vector<int> options, bool commit) {
    for (const auto& pair : session.actions.value()) {
        for (const auto& action : pair) {
            if (action.completed.value())
                continue;

            if (action.actorCellId.value() != session.localPlayerCellId.value())
                continue;

            if ((type == action_type::PICK && action.type.value() != "pick") ||
                (type == action_type::BAN && action.type.value() != "ban"))
                continue;

            if (action.championId.value())
                continue;
            
            if (const int championId = GetNextPick(options); championId != -1)
                return DoAction(static_cast<int>(action.id.value()), championId, commit)
                    ? championId : -1;
        }
    }
    
    return -1;
}

int feature::auto_picker::GetNextPick(const std::vector<int>& list) {
    for (const auto& id : list)
        if (!std::ranges::count(m_lobby_info.lockedChampions, id) && id != -1)
            return id;

    return -1;
}

bool feature::auto_picker::DoAction(int actionId, int championId, bool commit) {
    auto connectionManager = interface<connector_manager>::Get();

    const auto declareResult = connectionManager->MakeRequest(
        connector::request_type::PATCH,
        std::format("/lol-champ-select/v1/session/actions/{}", actionId),
        std::format("{}\"championId\":{}{}", "{", championId, "}")
    );

    if (declareResult.status != 204)
        return false;

    if (commit) {
        const auto commitResult = connectionManager->MakeRequest(
            connector::request_type::POST,
            std::format("/lol-champ-select/v1/session/actions/{}/complete", actionId),
            std::format("{}\"championId\":{}{}", "{", championId, "}")
        );

        if (commitResult.status != 204)
            return false;
    }

    return true;
}

void feature::auto_picker::NotifyUser(const std::string& title, const std::string& message) {
    interface<browser_manager>::Get()->CreateNotification(title, message, notification_type::NONE);
}