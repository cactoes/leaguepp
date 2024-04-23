#include "autopicker.hpp"

#include <utils.hpp>

#include "../interface/holder.hpp"
#include "../managers/configmanager.hpp"
#include "../managers/connectormanager.hpp"
#include "../managers/resourcemanager.hpp"

#include "../endpointmappers/champselectsession.hpp"
#include "../endpointmappers/lobbylobby.hpp"

#include "../clientinterfacer.hpp"

#undef interface

void feature::AutoPicker::Setup(std::shared_ptr<ui::Frame> frame, IUiFramework* frameworkApiHandle) {
    m_frameworkApiHandle = frameworkApiHandle;

    auto cfg = interface<ConfigManager>::Get()->Get(CONFIG_BASIC);
    m_cfg.autoPickerPreferredLineBlind = CVarHandle<std::string>(cfg, "autoPicker::sPreferredLineBlind");
    m_cfg.autoPickerEnabled = CVarHandle<bool>(cfg, "autoPicker::bEnabled");
    m_cfg.autoPickerEarlyDeclare = CVarHandle<bool>(cfg, "autoPicker::bEarlyDeclare");
    m_cfg.autoPickerMode = CVarHandle<int>(cfg, "autoPicker::nMode");
    m_cfg.autoPickerStrictness = CVarHandle<int>(cfg, "autoPicker::nStrictness");

    if (m_cfg.autoPickerPreferredLineBlind.Get().empty())
        m_cfg.autoPickerPreferredLineBlind.Set("top");

    m_cfg.autoPickerActiveLaneBans = CVarHandle<std::vector<int>>(cfg, ("autoPicker::" + m_cfg.autoPickerPreferredLineBlind.Get() + "::banIds"));
    m_cfg.autoPickerActiveLanePicks = CVarHandle<std::vector<int>>(cfg, ("autoPicker::" + m_cfg.autoPickerPreferredLineBlind.Get() + "::pickIds"));

    frame->AddCheckbox("enabled", NO_HINT, m_cfg.autoPickerEnabled.Get(),
        ui::checkbox_callback(&AutoPicker::OnSetEnabled, this));

    frame->AddCheckbox("early declare", NO_HINT, m_cfg.autoPickerEarlyDeclare.Get(),
        ui::checkbox_callback(&AutoPicker::OnSetEarlyDeclare, this));

    frame->AddSelector("bot mode", NO_HINT, m_cfg.autoPickerMode.Get(), m_modes,
        ui::selector_callback(&AutoPicker::OnBotModeUpdate, this));

    frame->AddSelector("lane strictness", "none - the lane doesnt matter\\nloose - primary / secondary (if available)\\nstrict - primary", m_cfg.autoPickerStrictness.Get(), m_strictnesses,
        ui::selector_callback(&AutoPicker::OnBotStrictnessUpdate, this));

    frame->AddDropdown("preferred lane (blind)", "the lane that gets used in blind pick modes", SINGLE, std::vector<std::string>{ m_cfg.autoPickerPreferredLineBlind.Get() }, clientinterfacer::lanes,
        ui::dropdown_callback(&AutoPicker::OnPrefferdLaneUpdate, this));

    frame->AddDropdown(
        "lane", "select the lane for the bans & picks input", SINGLE, std::vector<std::string>{ m_cfg.autoPickerPreferredLineBlind.Get() }, clientinterfacer::lanes,
        ui::dropdown_callback([this, frame](std::string item, bool, std::vector<std::string> list) {
            auto resourceManger = interface<ResourceManager>::Get();
            auto cfg = interface<ConfigManager>::Get()->Get(CONFIG_BASIC);

            m_cfg.autoPickerActiveLaneBans = CVarHandle<std::vector<int>>(cfg, ("autoPicker::" + item + "::banIds"));
            m_cfg.autoPickerActiveLanePicks = CVarHandle<std::vector<int>>(cfg, ("autoPicker::" + item + "::pickIds"));

            frame->GetComponent<ui::List>("autoPickerBans")->SetActiveItems(resourceManger->MapChampionIdsToNames(m_cfg.autoPickerActiveLaneBans.Get()));
            frame->GetComponent<ui::List>("autoPickerPicks")->SetActiveItems(resourceManger->MapChampionIdsToNames(m_cfg.autoPickerActiveLanePicks.Get()));
            return list;
        })
    );

    auto ListValidator = [](CVarHandle<std::vector<int>>& handle) {
        return ui::list_validator_callback([&](std::string v) {
            auto resourceManger = interface<ResourceManager>::Get();

            auto id = resourceManger->ChampionNameToId(v);
            if (id == 0)
                return false;

            const auto list = handle.Get();
            return std::ranges::find(list, id) == list.end();
        });
    };

    auto ListCallback = [](CVarHandle<std::vector<int>>& handle) {
        return ui::list_callback([&](std::vector<std::string> list) {
            auto resourceManger = interface<ResourceManager>::Get();

            std::vector<int> newItems = {};
            for (auto& v : list) {
                auto id = resourceManger->ChampionNameToId(v);
                v = resourceManger->ChampionIdToName(id);
                newItems.push_back(id);
            }

            handle.Set(newItems);
            return list;
        });
    };

    auto resourceManger = interface<ResourceManager>::Get();
    
    frame->AddList("bans", NO_HINT, resourceManger->MapChampionIdsToNames(m_cfg.autoPickerActiveLaneBans.Get()),
        ListValidator(m_cfg.autoPickerActiveLaneBans), ListCallback(m_cfg.autoPickerActiveLaneBans))
        ->SetId("autoPickerBans");

    frame->AddList("picks", NO_HINT, resourceManger->MapChampionIdsToNames(m_cfg.autoPickerActiveLanePicks.Get()),
        ListValidator(m_cfg.autoPickerActiveLanePicks), ListCallback(m_cfg.autoPickerActiveLanePicks))
        ->SetId("autoPickerPicks");

    auto connectorManager = interface<ConnectorManager>::Get();

    connectorManager->AddEventListener(
        "/lol-gameflow/v1/gameflow-phase",
        client_callback([this, connectorManager](std::string, nlohmann::json data) {
            m_lobby_info.isInChampSelect = data.get<std::string>() == "ChampSelect";

            // for early declare
            if (m_lobby_info.isInChampSelect && m_cfg.autoPickerEnabled.Get() && m_cfg.autoPickerEarlyDeclare.Get()) {
                const auto sessionDataResult = connectorManager->MakeRequest(connector::request_type::GET, "/lol-champ-select/v1/session");
                const auto lobbyDataResult = connectorManager->MakeRequest(connector::request_type::GET, "/lol-lobby/v2/lobby");

                if (sessionDataResult.status == 200 && lobbyDataResult.status == 200)
                    HandleFrame(sessionDataResult.data.get<champselect::Session>(), lobbyDataResult.data.get<lobby::Lobby>());
            }
        })
    );

    connectorManager->AddEventListener("/lol-champ-select/v1/session", client_callback([this, connectorManager](std::string, nlohmann::json data) {
        if (!m_cfg.autoPickerEnabled.Get())
            return;

        const auto lobbyDataResult = connectorManager->MakeRequest(connector::request_type::GET, "/lol-lobby/v2/lobby");
        if (lobbyDataResult.status != 200) {
            m_frameworkApiHandle->CreateNotification("error", "failed to get lobby data");
            return;
        }

        HandleFrame(data.get<champselect::Session>(), lobbyDataResult.data.get<lobby::Lobby>());
    }));
}

std::string feature::AutoPicker::GetName() {
    return "auto picker";
}

std::vector<int> feature::AutoPicker::GetLockedChampions(const champselect::Session& session) {
    std::vector<int> lockedChampions = {};
    for (const auto& pair : session.actions.value())
        for (const auto& action : pair)
            if (action.completed.value())
                lockedChampions.push_back(static_cast<int>(action.championId.value()));

    return lockedChampions;
}

feature::player_state feature::AutoPicker::GetPlayerState(const champselect::Session& session) {
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

feature::lane_state feature::AutoPicker::GetLaneState(const champselect::Session& session, const lobby::Lobby& lobby) {
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

    // FIXME: if lane is fill it will register as other position

    // do lane based checks
    if (StringCompareI(assignedPosition, lobby.localMember->firstPositionPreference.value()))
        return lane_state::ASSIGNED_PRIMARY_POSITION;

    if (StringCompareI(assignedPosition, lobby.localMember->secondPositionPreference.value()))
        return lane_state::ASSIGNED_SECONDARY_POSITION;

    return lane_state::OTHER_POSITION;
}

bool feature::AutoPicker::ValidateLaneState(lane_state state, int strictness) {
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
        // we cant pick a lane but we aren't concerned with it
        if (state == lane_state::NO_PICKABLE_LANE)
            return true;

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

void feature::AutoPicker::HandleFrame(const champselect::Session& session, const lobby::Lobby& lobby) {
    const int mode = m_cfg.autoPickerMode.Get();
    if (mode == BOT_MANUAL || !m_lobby_info.isInChampSelect)
        return;

    const int strictness = m_cfg.autoPickerStrictness.Get();
    const auto laneState = GetLaneState(session, lobby);
    if (!ValidateLaneState(laneState, strictness))
        return;

    m_lobby_info.lockedChampions = GetLockedChampions(session);

    // TODO: notif the user with the banned / picked champ name
    // NotifyUser("declared champion", "champion_name");

    std::string assignedPosition = CIGetAssignedPosition(session);
    if (assignedPosition.empty())
        assignedPosition = m_cfg.autoPickerPreferredLineBlind.Get();

    auto cfg = interface<ConfigManager>::Get()->Get(CONFIG_BASIC);

    switch (GetPlayerState(session)) {
        case player_state::DECLARING:
            (void)MakeAction(session, action_type::PICK, cfg->GetVar<std::vector<int>>(("autoPicker::" + assignedPosition + "::pickIds").c_str()), false);
            break;
        case player_state::BANNING:
            (void)MakeAction(session, action_type::BAN, cfg->GetVar<std::vector<int>>(("autoPicker::" + assignedPosition + "::banIds").c_str()), mode == BOT_AUTO);
            break;
        case player_state::PICKING:
            (void)MakeAction(session, action_type::PICK, cfg->GetVar<std::vector<int>>(("autoPicker::" + assignedPosition + "::pickIds").c_str()), mode == BOT_AUTO);
            break;
        case player_state::INVALID:
        case player_state::WAITING:
        default:
            break;
    }
}

int feature::AutoPicker::MakeAction(const champselect::Session& session, action_type type, const std::vector<int> options, bool commit) {
    const auto& localPlayerCellId = session.localPlayerCellId.value();

    for (const auto& pair : session.actions.value()) {
        for (const auto& action : pair) {
            if (action.completed.value())
                continue;

            if (action.actorCellId.value() != localPlayerCellId)
                continue;

            if ((type == action_type::PICK && action.type.value() != "pick") ||
                (type == action_type::BAN && action.type.value() != "ban"))
                continue;

            const int championId = GetNextPick(options);
            // const auto& selectedChampionId = action.championId.value();
            // if (selectedChampionId != 0 && selectedChampionId != championId)
            //     continue;
            
            if (championId != -1)
                return DoAction(static_cast<int>(action.id.value()), championId, commit)
                    ? championId : -1;
        }
    }
    
    return -1;
}

int feature::AutoPicker::GetNextPick(const std::vector<int>& list) {
    for (const auto& id : list)
        if (!std::ranges::count(m_lobby_info.lockedChampions, id) && id != -1)
            return id;

    return -1;
}

bool feature::AutoPicker::DoAction(int actionId, int championId, bool commit) {
    auto connectionManager = interface<ConnectorManager>::Get();

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

bool feature::AutoPicker::OnSetEnabled(bool state) {
    return m_cfg.autoPickerEnabled.Set(state);
}

bool feature::AutoPicker::OnSetEarlyDeclare(bool state) {
    return m_cfg.autoPickerEarlyDeclare.Set(state);
}

int feature::AutoPicker::OnBotModeUpdate(std::string mode) {
    return m_cfg.autoPickerMode.Set(static_cast<int>(std::distance(m_modes.begin(), std::ranges::find(m_modes, mode))));
}

int feature::AutoPicker::OnBotStrictnessUpdate(std::string strictness) {
    return m_cfg.autoPickerStrictness.Set(static_cast<int>(std::distance(m_strictnesses.begin(), std::ranges::find(m_strictnesses, strictness))));
}

std::vector<std::string> feature::AutoPicker::OnPrefferdLaneUpdate(std::string item, bool, std::vector<std::string> list) {
    m_cfg.autoPickerPreferredLineBlind.Set(item);
    return list;
}