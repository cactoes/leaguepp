#include "features/profile_controller.hpp"

#include "managers/config_manager.hpp"
#include "managers/league_connector_manager.hpp"
#include "managers/window_manager.hpp"
#include "endpoint_mappers.hpp"

bool update_profile(std::function<bool(lolchat::Me&)> setter) {
    auto lcm = manager::instance<league_connector_manager>();

    auto result = lcm->request<200>(connector::RT_GET, "/lol-chat/v1/me");
    if (!result.has_value())
        return false;

    auto data = result->get<lolchat::Me>();

    if (!setter(data))
        return false;

    return lcm->request<201>(connector::RT_PUT, "/lol-chat/v1/me", nlohmann::json(data).dump()).has_value();
}

void feature::profile_controller::setup(std::shared_ptr<reflection::component::abstract_frame> frame) {
    auto lcm = manager::instance<league_connector_manager>();
    auto cm = manager::instance<config_manager>();
    auto config = cm->get_config(USER_SETTINGS_CONFIG);

    frame->add_label("Profile tier");
    frame->add_selector("", lc::get_names(lc::tier::list), { (int)lc::get_index_of(config->get_cvar_u<std::string>("str_tier")->get(), lc::tier::list).value_or(0) }, [cm, config](auto, std::vector<int> active_items){
        auto new_tier = lc::tier::list.at(active_items.at(0)).name;
        cm->tracked_set_cvar<std::string>(config, "str_tier", new_tier);

        update_profile([new_tier](lolchat::Me& me_obj) {
            me_obj.lol->rankedLeagueQueue = lc::queue::queues.at(lc::queue::RANKED_SOLO_5x5).name;
            me_obj.lol->rankedLeagueTier = new_tier;
            return true;
        });
    });

    frame->add_label("Profile division");
    frame->add_selector("", lc::get_names(lc::division::list), { (int)lc::get_index_of(config->get_cvar_u<std::string>("str_division")->get(), lc::division::list).value_or(0) }, [cm, config](auto, std::vector<int> active_items){
        auto new_division = lc::division::list.at(active_items.at(0)).name;
        cm->tracked_set_cvar<std::string>(config, "str_division", new_division);

        update_profile([new_division](lolchat::Me& me_obj) {
            me_obj.lol->rankedLeagueQueue = lc::queue::queues.at(lc::queue::RANKED_SOLO_5x5).name;
            me_obj.lol->rankedLeagueDivision = new_division;
            return true;
        });
    });

    frame->add_label("Profile mastery points");
    frame->add_input(config->get_cvar_u<std::string>("str_mastery")->get(), [cm, config](auto, std::string v) {
        std::string fixed_mastery_score = std::to_string(lpp_core_utils::parse_number<uint64_t>(v).value_or(0));
        cm->tracked_set_cvar<std::string>(config, "str_mastery", fixed_mastery_score);

        update_profile([fixed_mastery_score](lolchat::Me& me_obj) {
            me_obj.lol->rankedLeagueQueue = lc::queue::queues.at(lc::queue::RANKED_SOLO_5x5).name;
            me_obj.lol->legendaryMasteryScore = fixed_mastery_score;
            return true;
        });

        return fixed_mastery_score;
    }, { .submit_button_text = "Set" });

    frame->add_checkbox("Auto update profile", config->get_cvar_u<bool>("b_auto_update_profile")->get(), [cm, config](auto, bool state){
        cm->tracked_set_cvar<bool>(config, "b_auto_update_profile", state);
    });

    auto full_update_profile = [config]() {
        return update_profile([config](lolchat::Me& me_obj) {
            me_obj.lol->rankedLeagueQueue = lc::queue::queues.at(lc::queue::RANKED_SOLO_5x5).name;
            me_obj.lol->rankedLeagueTier = config->get_cvar_u<std::string>("str_tier")->get();
            me_obj.lol->rankedLeagueDivision = config->get_cvar_u<std::string>("str_division")->get();
            me_obj.lol->legendaryMasteryScore = config->get_cvar_u<std::string>("str_mastery")->get();
            return true;
        });
    };

    // auto btn_frame = frame->add_frame("", { .outline = false, .layout = reflection::component::fl_horizontal });
    m_button_update_profile = frame->add_button("Update", [full_update_profile](auto) {
        if (!full_update_profile())
            wm::create_notification("Profile controller", "Failed to update profile...");
    }, { .disabled = true, .full_width = true }).value();

    m_button_clear_tokens = frame->add_button("Clear tokens", [](auto) {
        if (!lh::s_clear_tokens())
            wm::create_notification("Profile controller", "Failed to clear tokens...");
    }, { .disabled = true, .full_width = true }).value();

    lcm->add_endpoint_callback("/lol-chat/v1/me", [config, full_update_profile](std::string, nlohmann::json){
        if (config->get_cvar_u<bool>("b_auto_update_profile")->get())
            full_update_profile();
    });

    lcm->add_connect_handler([this](){
        m_button_update_profile->set_disabled(false);
        m_button_clear_tokens->set_disabled(false);
    });

    lcm->add_disconnect_handler([this](){
        m_button_update_profile->set_disabled(true);
        m_button_clear_tokens->set_disabled(true);
    });
}