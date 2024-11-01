#include "features/lobby_controller.hpp"

#include "managers/config_manager.hpp"
#include "managers/league_connector_manager.hpp"
#include "managers/window_manager.hpp"
#include "managers/console_manager.hpp"
#include "endpoint_mappers.hpp"

void feature::lobby_controller::setup(std::shared_ptr<reflection::component::abstract_frame> frame) {
    auto lcm = manager::instance<league_connector_manager>();
    auto cm = manager::instance<config_manager>();
    auto config = cm->get_config(USER_SETTINGS_CONFIG);

    frame->add_label("Auto dodge lanes");
    frame->add_selector("Auto dodge lanes", lc::get_names(lc::lanes::list), config->get_cvar_u<std::vector<int>>("vec_auto_dodge_lanes")->get(), [cm, config](auto, std::vector<int> new_values) {
        cm->tracked_set_cvar<std::vector<int>>(config, "vec_auto_dodge_lanes", new_values);
    }, { .is_multi = true });

    frame->add_checkbox("Auto dodge", config->get_cvar_u<bool>("b_auto_dodge")->get(), [cm, config](auto, bool state) {
        cm->tracked_set_cvar<bool>(config, "b_auto_dodge", state);
    });

    frame->add_checkbox("Auto accept", config->get_cvar_u<bool>("b_auto_accept")->get(), [cm, config](auto, bool state) {
        cm->tracked_set_cvar<bool>(config, "b_auto_accept", state);
    });

    frame->add_checkbox("Auto search", config->get_cvar_u<bool>("b_auto_search")->get(), [cm, config](auto, bool state) {
        cm->tracked_set_cvar<bool>(config, "b_auto_search", state);
    });

    m_button_dodge = frame->add_button("Dodge", [](auto) {
        if (!lh::dodge_queue())
            manager::instance<console_manager>()->add_log("[lc] Failed to dodge match...");
    }, { .disabled = true, .full_width = true }).value();

    lcm->add_endpoint_callback("/lol-gameflow/v1/gameflow-phase", [this](std::string, nlohmann::json data) {
        const auto gameflow_hash = lc::hash_constant(data.get<std::string>());
        
        handle_auto_accept(gameflow_hash);
        handle_auto_dodge(gameflow_hash);
        handle_auto_search(gameflow_hash);

        m_button_dodge->set_disabled(gameflow_hash != lc::gameflow::champselect);
    });
}

void feature::lobby_controller::handle_auto_accept(uint64_t gameflow_hash) {
    auto cm = manager::instance<config_manager>();
    auto config = cm->get_config(USER_SETTINGS_CONFIG);

    if (gameflow_hash != lc::gameflow::readycheck || !config->get_cvar_u<bool>("b_auto_accept"))
        return;

    if (!lh::accept_match())
        manager::instance<console_manager>()->add_log("[lc] Failed to accept match...");
}

void feature::lobby_controller::handle_auto_dodge(uint64_t gameflow_hash) {
    auto lcm = manager::instance<league_connector_manager>();
    auto cm = manager::instance<config_manager>();
    auto config = cm->get_config(USER_SETTINGS_CONFIG);

    // check gameflow state && check if config item is enabled
    if (gameflow_hash != lc::gameflow::champselect || !config->get_cvar_u<bool>("b_auto_dodge")->get())
        return;

    const std::vector<int>& lane_dodge_ids = config->get_cvar_u<std::vector<int>>("vec_auto_dodge_lanes")->get();
    
    // no lane to dodge on
    if (lane_dodge_ids.size() == 0)
        return;

    // dodge on all lanes
    if (lane_dodge_ids.size() == lc::lanes::list.size()) {
        if (!lh::dodge_queue())
            manager::instance<console_manager>()->add_log("[lc] Failed to dodge match...");

        return;
    }

    auto result = lcm->request<200>(connector::request_type::GET, "/lol-champ-select/v1/session");
    if (!result.has_value())
        return;

    auto assigned_pos = lh::get_assigned_position(result.value().get<champselect::Session>());
    if (!assigned_pos.has_value())
        return;

    // check if assigned lane is in dodge list
    if (std::ranges::find(lane_dodge_ids, assigned_pos->index) == lane_dodge_ids.end())
        return;

    if (!lh::dodge_queue())
        manager::instance<console_manager>()->add_log("[lc] Failed to dodge match...");
}

void feature::lobby_controller::handle_auto_search(uint64_t gameflow_hash) {
    auto cm = manager::instance<config_manager>();
    auto config = cm->get_config(USER_SETTINGS_CONFIG);

    if (gameflow_hash != lc::gameflow::lobby || !config->get_cvar_u<bool>("b_auto_search")->get())
        return;

    if (!lh::start_search())
        manager::instance<console_manager>()->add_log("[lc] Failed to start search...");
}