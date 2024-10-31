#include "features/champion_select_controller.hpp"

#include "managers/config_manager.hpp"
#include "managers/league_connector_manager.hpp"
#include "managers/window_manager.hpp"
#include "endpoint_mappers.hpp"

#include <iostream>

feature::state_manager::lane_t feature::state_manager::get_lane_state(const champselect::Session& session, const lobby::Lobby& lobby) {
    // verify game config
    if (!lobby.gameConfig.has_value())
        return lane_t::L_INVALID;

    // verify localMember
    if (!lobby.localMember.has_value())
        return lane_t::L_INVALID;

    // verify game type
    const auto& queue_id = lobby.gameConfig->queueId.value();
    if (!lobby.gameConfig->isCustom.value())
        return lane_t::L_IMPOSSIBLE;

    // verify queue id
    auto queue_struct = lc::queue::get_queue_by_id(queue_id);
    if (!queue_struct.has_value())
        return lane_t::L_IMPOSSIBLE;

    if (queue_struct->get().id != lc::queue::queues.at(lc::queue::RANKED_FLEX_SR).id &&
        queue_struct->get().id != lc::queue::queues.at(lc::queue::RANKED_SOLO_5x5).id &&
        queue_struct->get().id != lc::queue::queues.at(lc::queue::NORMAL_DRAFT).id)
        return lane_t::L_IMPOSSIBLE;

    // get assigned lane
    auto assigned_position = lh::get_assigned_position(session);
    if (!assigned_position.has_value())
        return lane_t::L_INVALID;

    // verify assigned lane
    if (lpp_core_utils::strcmpi(assigned_position->name, lobby.localMember->firstPositionPreference.value()))
        return lane_t::L_PRIMARY;

    if (lpp_core_utils::strcmpi(assigned_position->name, lobby.localMember->secondPositionPreference.value()))
        return lane_t::L_SECONDARY;

    return lane_t::L_OTHER;
}

bool feature::state_manager::check_lane_state(lane_t lane, champion_select_controller::strictness_t strictness) {
    // strictness S_NONE we dont care abt what lane we are in
    if (strictness == champion_select_controller::strictness_t::S_NONE)
        return true;

    // the lane aint there qq
    if (lane == lane_t::L_INVALID)
        return false;

    // strictness S_LOOSE we only want to run if we have our secondary or primary lane
    if (strictness == champion_select_controller::strictness_t::S_LOOSE) {
        // we cant pick a lane but we aren't concerned with it
        if (lane == lane_t::L_IMPOSSIBLE)
            return true;

        // check if we are primary or secondary
        return lane == lane_t::L_PRIMARY ||
            lane == lane_t::L_SECONDARY;
    }

    // here we have strictness S_STRICT
    // wich mean we only want to run when we have our primary lane
    return lane == lane_t::L_PRIMARY;
}

feature::state_manager::phase_t feature::state_manager::get_phase(const champselect::Session& session) {
    const std::string& current_phase = session.timer->phase.value();
    const uint64_t current_phase_hash = lc::hash_constant(current_phase.c_str());

    if (current_phase_hash == lc::hash_constant("PLANNING"))
        return phase_t::P_DECLARING;

    if (current_phase_hash == lc::hash_constant("FINALIZATION"))
        return phase_t::P_WAITING;

    if (current_phase_hash == lc::hash_constant("BAN_PICK")) {
        const auto& local_player_cell_id = session.localPlayerCellId.value();

        for (const auto& pair : session.actions.value()) {
            for (const auto& action : pair) {
                if (action.completed.value())
                    continue;

                if (action.actorCellId.value() != local_player_cell_id)
                    continue;

                if (action.isInProgress.value())
                    return action.type.value() == "ban" ? phase_t::P_BANNING : phase_t::P_PICKING;

                return phase_t::P_WAITING;
            }
        }
    }

    return phase_t::P_INVALID;
}

std::optional<feature::state_manager::action_t> feature::state_manager::get_current_state(const champselect::Session& session, const lobby::Lobby& lobby, champion_select_controller::mode_t mode, champion_select_controller::strictness_t strictness) {
    // assumes we are in champion select
    if (mode == champion_select_controller::mode_t::M_MANUAL)
        return std::nullopt;

    // check our lane
    const lane_t lane_state = get_lane_state(session, lobby);
    if (!check_lane_state(lane_state, strictness))
        return std::nullopt;

    switch (get_phase(session)) {
        case phase_t::P_DECLARING:
            return action_t{ .type = action_type_t::AT_PICK, .commit = false };
        case phase_t::P_BANNING:
            return action_t{ .type = action_type_t::AT_BAN, .commit = mode == champion_select_controller::mode_t::M_AUTO };
        case phase_t::P_PICKING:
            return action_t{ .type = action_type_t::AT_PICK, .commit = mode == champion_select_controller::mode_t::M_AUTO };
        case phase_t::P_INVALID:
        case phase_t::P_WAITING:
        default:
            return std::nullopt;
    }
}

void feature::champion_select_controller::setup(std::shared_ptr<reflection::component::abstract_frame> frame) {
    auto lcm = manager::instance<league_connector_manager>();
    auto cm = manager::instance<config_manager>();
    auto config = cm->get_config(USER_SETTINGS_CONFIG);

    frame->add_label("Mode");
    frame->add_selector("Mode", m_mode, { config->get_cvar_u<mode_t>("e_csc_state_manager_mode")->get() }, [cm, config, this](auto, std::vector<int> active_items) {
        auto new_mode = static_cast<mode_t>(active_items.at(0));
        cm->tracked_set_cvar<mode_t>(config, "e_csc_state_manager_mode", new_mode);
    });

    frame->add_label("Strictness");
    frame->add_selector("Strictness", m_strictness, { config->get_cvar_u<strictness_t>("e_csc_state_manager_strictness")->get() }, [cm, config, this](auto, std::vector<int> active_items) {
        auto new_strictness = static_cast<strictness_t>(active_items.at(0));
        cm->tracked_set_cvar<strictness_t>(config, "e_csc_state_manager_strictness", new_strictness);
    });

    lcm->add_endpoint_callback("/lol-gameflow/v1/gameflow-phase", [this](std::string, nlohmann::json data) {
        if (lc::hash_constant(data.get<std::string>()) != lc::gameflow::champselect)
            return;

        auto lcm = manager::instance<league_connector_manager>();
        const auto session_data_result = lcm->request<200>(connector::RT_GET, "/lol-champ-select/v1/session");
        const auto lobby_data_result = lcm->request<200>(connector::RT_GET, "/lol-lobby/v2/lobby");

        if (!lobby_data_result.has_value() || !session_data_result.has_value())
            return;

        handle_frame(session_data_result->get<champselect::Session>(), lobby_data_result->get<lobby::Lobby>());
    });

    lcm->add_endpoint_callback("/lol-champ-select/v1/session", [this](std::string, nlohmann::json session_data) {
        auto lcm = manager::instance<league_connector_manager>();
        const auto lobby_data_result = lcm->request<200>(connector::RT_GET, "/lol-lobby/v2/lobby");

        if (!lobby_data_result.has_value())
            return;

        handle_frame(session_data.get<champselect::Session>(), lobby_data_result->get<lobby::Lobby>());
    });
}

std::vector<int> feature::champion_select_controller::get_locked_champions(const champselect::Session& session) {
    std::vector<int> locked_champions = {};
    for (const auto& pair : session.actions.value())
        for (const auto& action : pair)
            if (action.completed.value())
                locked_champions.push_back(static_cast<int>(action.championId.value()));

    return locked_champions;
}

int feature::champion_select_controller::get_next_id(const std::vector<int>& locks, const std::vector<int>& list) {
    for (const auto& id : list)
        if (!std::ranges::count(locks, id) && id != -1)
            return id;

    return -1;
}

void feature::champion_select_controller::handle_frame(const champselect::Session& session, const lobby::Lobby& lobby) {
    auto lcm = manager::instance<league_connector_manager>();
    auto cm = manager::instance<config_manager>();
    auto config = cm->get_config(USER_SETTINGS_CONFIG);

    auto mode = config->get_cvar_u<mode_t>("e_csc_state_manager_mode")->get();
    auto strictness = config->get_cvar_u<strictness_t>("e_csc_state_manager_strictness")->get();

    const auto state = state_manager::get_current_state(session, lobby, mode, strictness);
    if (!state.has_value())
        return;

    auto locked_champions = get_locked_champions(session);
    auto assigned_position = lh::get_assigned_position(session);

    if (!assigned_position.has_value())
        return;

    const std::vector<int>& picks = config->get_cvar_u<std::vector<int>>("vec_csc_picks_" + std::to_string(assigned_position->index))->get();
    const std::vector<int>& bans = config->get_cvar_u<std::vector<int>>("vec_csc_bans_" + std::to_string(assigned_position->index))->get();

    int next_pick = get_next_id(locked_champions, picks);
    int next_ban = get_next_id(locked_champions, bans);

    switch (state->type) {
        case state_manager::action_type_t::AT_PICK:
            break;
        case state_manager::action_type_t::AT_BAN:
            break;
        default:
            break;
    }

    // TODO

    // if (state.has_value())
    //     std::cout << "{ state->type: " << state->type << ", state->commit: " << state->commit << " }\n";
}