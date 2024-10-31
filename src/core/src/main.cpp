#include <iostream>
#include <reflection/reflection.hpp>

#include "managers/window_manager.hpp"
#include "managers/league_connector_manager.hpp"
#include "managers/config_manager.hpp"
#include "managers/feature_manager.hpp"
#include "managers/resource_manager.hpp"
#include "managers/manager.hpp"

#include "features/lobby_controller.hpp"
#include "features/profile_controller.hpp"
#include "features/champion_select_controller.hpp"

#define IDI_ICON_DEFAULT 1
#define IDI_ICON_DISCONNECTED 2
#define IDI_ICON_CONNECTED 3

void _m_main(HINSTANCE) {
    std::shared_ptr<config_manager> cm = manager::instance<config_manager>();
    config_handle _config_handle = cm->create_config(USER_SETTINGS_CONFIG);
    
    _config_handle->add_template<bool>("b_auto_accept");
    _config_handle->add_template<bool>("b_auto_dodge");
    _config_handle->add_template<bool>("b_auto_search");
    _config_handle->add_template<std::vector<int>>("vec_auto_dodge_lanes");

    _config_handle->add_template<std::string>("str_tier");
    _config_handle->add_template<std::string>("str_division");
    _config_handle->add_template<std::string>("str_mastery");
    _config_handle->add_template<bool>("b_auto_update_profile");

    _config_handle->add_template<feature::champion_select_controller::mode_t>("e_csc_state_manager_mode");
    _config_handle->add_template<feature::champion_select_controller::strictness_t>("e_csc_state_manager_strictness");
    _config_handle->add_template<int>("i_pref_line_blind");
    _config_handle->add_template<bool>("b_auto_picker_enabled");
    _config_handle->add_template<bool>("b_early_declare_enabled");

    for (auto& lane : lc::lanes::list) {
        _config_handle->add_template<std::vector<int>>("vec_csc_picks_" + std::to_string(lane.index));
        _config_handle->add_template<std::vector<int>>("vec_csc_bans_" + std::to_string(lane.index));
    }

    if (!cm->load_config(_config_handle))
        cm->dump_config(_config_handle);

    manager::instance<resource_manager>()->setup();

    reflection::component::frame_options_t options {};
    options.layout = reflection::component::fl_horizontal;
    
    wm::window_options_t window_options {};
    window_options.name = "League++";

    std::shared_ptr<reflection::browser_window> window = wm::create_window(options, window_options);

    auto main_frame = window->get_frame();

    auto left_frame = main_frame->add_frame("", { .outline = false, .max_size = true });
    auto csc_frame = left_frame->add_frame("Champion select controller", { .outline = true, .max_size = true });
    
    auto right_frame = main_frame->add_frame("", { .outline = false, .max_size = true });
    auto pc_frame = right_frame->add_frame("Profile controller", { .outline = true, .max_size = false });
    auto lc_frame = right_frame->add_frame("Lobby controller", { .outline = true, .max_size = true });

    // std::string m_current_color = "#728ab3";
    // frame1_1_parent->add_input(m_current_color, [&](auto, std::string v) {
    //     m_current_color = lpp_core_utils::test(v, "^#([a-fA-F0-9]{6})$") ? v : m_current_color;
    //     window->set_color(m_current_color);
    //     return m_current_color;
    // });

    auto lcm = manager::instance<league_connector_manager>();

    lcm->setup();

    lcm->add_connect_handler([&]() {
        window->set_icon(IDI_ICON_CONNECTED);
    });

    lcm->add_disconnect_handler([&]() {
        window->set_icon(IDI_ICON_DISCONNECTED);
    });

    auto fm = manager::instance<feature_manager>();

    fm->add_feature<feature::lobby_controller>(lc_frame);
    fm->add_feature<feature::profile_controller>(pc_frame);
    fm->add_feature<feature::champion_select_controller>(csc_frame);

    window->start();

    window->register_event_handler(reflection::event_t::E_ON_RENDER_FINISHED, [](auto) {
        manager::instance<league_connector_manager>()->setup();
    });

    MSG msg{};
    while (GetMessageA(&msg, nullptr, 0, 0) != 0) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    lcm->shutdown();
}

int main(int, char**) {
    _m_main(GetModuleHandle(nullptr));
    return 0;
}

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int) {
    _m_main(instance);
    return 0;
}